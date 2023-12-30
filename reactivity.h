#ifndef REACTIVITYTESTING_REACTIVITY_H
#define REACTIVITYTESTING_REACTIVITY_H

#include "debug.h"
#include "priority_queue.h"

struct Binding {
    void *bound_datum;
    function<void(void)> update;
    Fn* queue_stale_listener;
    unordered_set<void *> dependencies; // (not all have bindings) need for recalculating priority
    unordered_set<void *> dependants; // (all have bindings) need for what to update
    int priority;
};

bool compare_binding(Binding *left, Binding *right);

extern unordered_map<void *, unordered_set<Fn *> > data_listeners;
extern unordered_map<void *, Binding *> bindings;
extern PriorityQueue<Binding *, compare_binding> bindings_to_update;
extern int data_sync_active;

template<typename T>
void create_datum_listener_run_now(T *datum, Fn *datum_listener) {
    create_datum_listener(datum, datum_listener);
    datum_listener->f();
}

template<typename T>
void create_datum_listener(T *datum, Fn *datum_listener) {

//    ensure_msg(data_listeners[datum] != NULL, "datum is not listenable\n");
    ensure_msg(!data_listeners[datum].contains(datum_listener), "datum_listeners already contains datum_listener\n");

    data_listeners[datum].insert(datum_listener);
}

template<typename T>
void destroy_datum_listener(T *datum, Fn *datum_listener) {
//    assertm(datum_listeners, "datum is not listenable\n");
    ensure_msg(data_listeners[datum].contains(datum_listener), "datum_listeners does not contain datum_listener\n");

    data_listeners[datum].erase(datum_listener);
}

void trigger_listeners(void *datum);

template<typename T>
void update_listenable(T *listenable, T new_value) {
    ensure_msg(data_sync_active, "can only call update_listenable in data_sync block");
    *listenable = new_value;
    trigger_listeners(listenable);
}

template<typename T>
void queue_dependant_stale(T *dependant) {
    bindings_to_update.push(bindings[dependant]);
}

void recalculate_priority(Binding *binding);

template<typename T>
void create_binding(T *bound_datum, unordered_set<void *> dependencies, function<void(void)> update) {
    ensure_msg(data_sync_active, "not in a data_sync block");
    ensure_msg(!bindings.contains(bound_datum), "binding for this address already exists");

    auto queue_stale_listener = new Fn([=]() {
        queue_dependant_stale(bound_datum);
    });

    for (void *dependency: dependencies) {
        create_datum_listener(dependency, queue_stale_listener);

        if (!bindings.contains(dependency)) continue;

        Binding *dependency_binding = bindings[dependency];
        dependency_binding->dependants.insert(bound_datum);
    }

    auto *binding = new Binding{
            .bound_datum = bound_datum,
            .update = update,
            .queue_stale_listener = queue_stale_listener,
            .dependencies = dependencies,
            // .dependants = dependants,
            // .priority = greatest_priority + 1
    };
    recalculate_priority(binding);
    bindings[bound_datum] = binding;

    queue_dependant_stale(bound_datum);
}

template<typename T>
void destroy_binding(T *bound_datum) {
    ensure_msg(data_sync_active, "not in a data_sync block");
    ensure_msg(bindings.contains(bound_datum), "no binding for this address exists");

    Binding *binding = bindings[bound_datum];
    for (auto dependency: binding->dependencies) {
        data_listeners[dependency].erase(binding->queue_stale_listener);
        if (!bindings.contains(dependency)) continue;

        bindings[dependency]->dependants.erase(bound_datum);
    }

    bindings.erase(bound_datum);

    for (auto dependant: binding->dependants) {
        recalculate_priority(bindings[dependant]);
    }

    delete binding;
}

void begin_data_sync();

void end_data_sync();

void debug_list_reactivity();

#endif //REACTIVITYTESTING_REACTIVITY_H
