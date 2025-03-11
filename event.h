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

void debug_print(const string &s, const vector<void *> &stuff);
void end_scope();

template<typename T>
T *create(T *object, string name) {
    object_names.insert({object, name});
    return object;
}

template<typename T>
void destroy(T *object) {
    debug_print("destroy", {object});

    if(!object_names.contains(object)) {
        printf("Object does not exist or was already freed\n");
        abort();
    }
    object_names.erase(object);
    delete object;

    end_scope();
}

void provide_lifetime(void *object, Event *destroyer);
string name_of(void *object);

template<typename T>
void default_lifetime(T *object) {
    provide_lifetime(object, create(C Event([=]() {
        destroy(object);
    }), "[" + name_of(object) + "]_destroyer"));
}

void ugh_test();

void subsume_lifetime_under_lifetime(void *subsumed, void *dominant);

void assign_datum(int *datum, int value);

#endif //OPTO_EVENT_H
