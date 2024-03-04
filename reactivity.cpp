#include "globals.h"
#include "reactivity.h"
#include "debug.h"

bool compare_binding(Binding *left, Binding *right) {
    return left->priority > right->priority;
}

unordered_map<void *, unordered_set<Fn *> > data_listeners;
unordered_map<void *, Binding *> bindings;
PriorityQueue<Binding *, compare_binding> bindings_to_update;

void recalculate_priority(Binding *binding) {
    int greatest_priority = 0;
    for (void *dependency: binding->dependencies) {
        if (bindings.contains(dependency)) {
            Binding *dependency_binding = bindings[dependency];
            greatest_priority = max(dependency_binding->priority, greatest_priority);
        }
    }

    binding->priority = greatest_priority + 1;

    for (void *dependant: binding->dependants) {
        recalculate_priority(bindings[dependant]); // TODO use queues n stuff
    }
}

void trigger_listeners(void *datum) {
    ensure_msg(data_listeners.contains(datum), "datum is not listenable");

    auto datum_listeners = data_listeners[datum];
    for (auto datum_listener: datum_listeners) {
        datum_listener->f();
    }
}

void end_data_sync() {
    while (!bindings_to_update.is_empty()) {
        Binding *binding = bindings_to_update.pop();

        binding->update();
        if (data_listeners.contains(binding->bound_datum)) {
            trigger_listeners(binding->bound_datum);
        }
    }
}

void debug_list_reactivity() {
    printf("bindings: %d\n", (int) bindings.size());
}
