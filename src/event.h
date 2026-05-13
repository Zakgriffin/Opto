#pragma once

#ifndef OPTO_EVENT_H
#define OPTO_EVENT_H

#include "stds.h"
#include "globals.h"

typedef struct {
    unordered_set<function<void(void)> *> on_change;
} Signal;

typedef struct {
    function<void(void)> *f;
    vector<Signal *> signals;
} Listener;

extern Signal draw_visuals;
extern Signal input_listeners;

void signal_update(Signal *signal);

Listener create_listener(const vector<Signal*>& signals, function<void(void)> *f);
Listener create_listener_lazy(const vector<Signal*>& signals, function<void(void)> *f);

void destroy_listener(const Listener& listener);

//struct SequencedEvent {
//    SequencedEvent *prev;
//    SequencedEvent *next;
//    Event *event;
//};
//
//struct PropagatedEvent {
//    unordered_set<PropagatedEvent *> before;
//    unordered_set<PropagatedEvent *> after;
//    int order;
//    Event *event;
//};
//
//extern map<void *, string> object_names;
//extern map<void *, PropagatedEvent *> datum_to_update_propagated_event;
//
//#define C new
//
//void debug_print(const string &s, const vector<void *> &stuff);
//
//void end_scope();
//
//template<typename T>
//T *create(T *object, string name) {
//    object_names.insert({object, name});
//    return object;
//}
//
//template<typename T>
//void destroy(T *object) {
//    debug_print("destroy", {object});
//
//    if (!object_names.contains(object)) {
//        printf("Object does not exist or was already freed\n");
//        abort();
//    }
//    object_names.erase(object);
//    delete object;
//
//    end_scope();
//}
//
//void run_all_events();
//
//void provide_lifetime(void *object, Event *destroyer);
//
//string name_of(void *object);
//
//void schedule_propagated_event(PropagatedEvent *propagated_event);
//
//void schedule_all_recurring_events_next(const unordered_set<Event *> &events);
//
//void schedule_recurring_event_next(Event *event);
//
//void schedule_recurring_event_last(Event *event);
//
//PropagatedEvent *create_datum_update_propagated_event(void *datum, Event *datum_update_event, const unordered_set<void *> &dependencies);
//
//
//void make_observed_datum(void *datum);
//
//template<typename T>
//void make_observed_structure(T* structure, vector<void*> fields) {
//    auto destroyer = create(C Event([=]() {
//        destroy(structure);
//    }), "[" + name_of(structure) + "]_struct_destroyer");
//
//    provide_lifetime(structure, destroyer);
//
//    for(auto field : fields) {
//        provide_lifetime(field, destroyer);
//        make_observed_datum(field);
//    }
//}
//
//void initialize_garbo();
//
//template<typename T>
//void default_lifetime(T *object) {
//    provide_lifetime(object, create(C Event([=]() {
//        destroy(object);
//    }), "[" + name_of(object) + "]_destroyer"));
//}
//
//void ugh_test();
//
//void subsume_lifetime_under_lifetime(void *subsumed, void *dominant);
//
//template<typename T>
//void assign_datum(T *datum, T value) {
//    debug_print("assign_datum", {datum});
//
//    *datum = value;
//    schedule_propagated_event(datum_to_update_propagated_event.at(datum));
//
//    run_all_events();
//
//    end_scope();
//}
//
#endif //OPTO_EVENT_H
