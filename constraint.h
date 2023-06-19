#ifndef OPTO_CONSTRAINT_H
#define OPTO_CONSTRAINT_H

#include <vector>
#include <queue>
#include <unordered_map>
#include "priority_queue.h"
#include "globals.h"

using namespace std;

struct BindingInfo {
    void *bound_data;
    function<void(void)> update_binding;
    int priority;
    vector<void *> dependants;
    vector<void *> dependencies;

    F *to_trigger_me;
};

bool compare_binding_info(BindingInfo *left, BindingInfo *right);

extern unordered_map<void *, vector<F *> *> listeners;
extern unordered_map<void *, BindingInfo *> bindings;
extern PriorityQueue<BindingInfo *, compare_binding_info> bindings_to_update;

template<typename T>
void flag_dependant_stale(T *dependant) {
    bindings_to_update.push(bindings[dependant]);
}

template<typename T>
void update_listenable(T *listenable, T value) {
    *listenable = value;
    for (auto x: *listeners[listenable]) {
        x->f();
    }
}

template<typename T>
void create_listener(T *data, F *listener) {
    if (!listeners[data]) listeners[data] = new vector<F *>;
    listeners[data]->push_back(listener);

    listener->f();
}

template<typename T>
void create_listener_wait(T *data, F *listener) {
    if (!listeners[data]) listeners[data] = new vector<F *>;
    listeners[data]->push_back(listener);
}

template<typename T>
void destroy_listener(T *data, F *listener) {
    if (!listeners[data]) return;

    auto x = listeners[data];
    x->erase(find(x->begin(), x->end(), listener));
    if(x->size() == 0) listeners.erase(data);
}

template<typename T>
void create_binding(T *bound_data, vector<void *> dependencies, function<void(void)> update_binding) {
    if (bindings.contains(bound_data)) {
        printf("binding for this address already exists");
        abort();
    }

    auto *binding_info = new BindingInfo{
            .bound_data = bound_data,
            .update_binding = update_binding,
            .dependencies = dependencies,
            .to_trigger_me = new F([=]() {
                flag_dependant_stale(bound_data);
            }),
    };

    int greatest_priority = 0;
    for (void *dependency: dependencies) {
        if (bindings.contains(dependency)) {
            BindingInfo *dependency_binding_info = bindings[dependency];
            dependency_binding_info->dependants.push_back(bound_data);
            greatest_priority = max(dependency_binding_info->priority, greatest_priority);
        }

        create_listener_wait(dependency, binding_info->to_trigger_me);
    }

    binding_info->priority = greatest_priority + 1;

    bindings[bound_data] = binding_info;

    flag_dependant_stale(bound_data);
}

template<typename T>
void destroy_binding(T *address) {
    BindingInfo *binding_info = bindings[address];

    for (void *dependency: binding_info->dependencies) {

        // remove dependency's dependant (address)
        if (bindings.contains(dependency)) {
            auto d = bindings[dependency]->dependants;
            for (int i = 0; i < d.size(); i++) {
                if (d[i] == address) {
                    d.erase(d.begin() + i);
                }
            }
        }

        // remove dependency's listener
        vector<F *> *dependency_listeners = listeners[dependency];
        for (int i = 0; i < dependency_listeners->size(); i++) {
            if ((*dependency_listeners)[i] == binding_info->to_trigger_me) {
                dependency_listeners->erase(dependency_listeners->begin() + i);
            }
        }
    }

    delete binding_info;
    bindings.erase(address);
}

void update_all_tracked();

void constraint_testing();

void debug_list_constraint();


class CleanupContext {
    vector<function<void(void)>> to_clean;

public:
    void destroy_all() {
        for(const auto& x : to_clean) {
            x();
        }
    }

    template<typename T>
    void make_binding(T *bound_data, vector<void *> dependencies, function<void(void)> update_binding) {
        create_binding<T>(bound_data, dependencies, update_binding);
        to_clean.push_back([=]() { destroy_binding(bound_data); });
    }

    template<typename T>
    void make_listener(T *data, F *listener) {
        create_listener(data, listener);
        to_clean.push_back([=]() { destroy_listener(data, listener); });
    }
};

#endif //OPTO_CONSTRAINT_H
