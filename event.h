#ifndef OPTO_EVENT_H
#define OPTO_EVENT_H

#include "stds.h"
#include "globals.h"

struct SequencedEvent {
    SequencedEvent *prev;
    SequencedEvent *next;
    Event *event;
};

struct PropagatedEvent {
    unordered_set<PropagatedEvent *> before;
    unordered_set<PropagatedEvent *> after;
    int order;
    Event *event;
};

extern map<void *, string> object_names;

#define C new

template<typename T>
T *create(T *object, string name) {
    object_names.insert({object, name});
    return object;
}

template<typename T>
void destroy(T *object) {
    object_names.erase(object);
    delete object;
}

void ugh_test();

void subsume_lifetime_under_lifetime(void *subsumed, void *dominant);

void assign_datum(int *datum, int value);

#endif //OPTO_EVENT_H
