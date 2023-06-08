#ifndef OPTO_CONSTRAINT_H
#define OPTO_CONSTRAINT_H

#include <vector>
#include <queue>
#include <unordered_map>
#include "globals.h"

using namespace std;

extern unordered_map<void *, vector<F *> *> listeners;

struct BindingInfo {
    void *bound_data;
    function<void(void)> update_binding;
    int priority;
    vector<void *> dependants;
    vector<void *> dependencies;

    F *to_trigger_me;
};

template<typename T>
void make_listenable(T *address);

//template<typename T>
//void unmake_listenable(T *address);

// WHY DO I NEED TO PUT THIS IN HEADER
template<typename T>
void unmake_listenable(T *address) {
    for (auto x: *listeners[address]) {
        delete x;
    }
    listeners.erase(address);
}

template<typename T>
void update_listenable(T *listenable, T value);

template<typename T>
void create_binding(T *bound_data, vector<void *> dependencies, function<void(void)> update_expression);

template<typename T>
void destroy_binding(T *bound_data);

void update_all_tracked();

void constraint_testing();

void debug_list_constraint();

#endif //OPTO_CONSTRAINT_H
