#include "event.h"

int debug_scope = 0;

map<void *, string> object_names;

SequencedEvent *finished_event = create(C SequencedEvent, "finished_event");
SequencedEvent *current_sequenced_event = finished_event;

map<Event *, PropagatedEvent *> event_to_propagated_event;

map<int *, PropagatedEvent *> datum_to_update_propagated_event;
map<void *, PropagatedEvent *> datum_to_destroy_propagated_event;

string name_of(void *object) {
    if (object_names.contains(object)) {
        return object_names.at(object);
    }
    return "?";
}

void print_sequenced_events() {
    auto start = current_sequenced_event;
    auto s = start;

    do {
        printf("-> %s\n", name_of(s).c_str());
//        printf("      prev: %s\n", name_of(s->prev).c_str());
//        printf("      next: %s\n", name_of(s->next).c_str());

        if (event_to_propagated_event.contains(s->event)) {
            auto k = event_to_propagated_event.at(s->event);
            for (auto a: k->after) {
                printf("    THEN: %s\n", name_of(a).c_str());
            }
            printf("     PROP_AKA: %s\n", name_of(k).c_str());
        }

        s = s->next;
    } while (s != start);

    printf("\n");
}

void debug_print(const string &s, const vector<void *> &stuff) {
    debug_scope++;
    for (int i = 0; i < debug_scope; i++) printf("  ");

    printf("%s(", s.c_str());
    for (auto v: stuff) {
        printf(" %s,", name_of(v).c_str());
    }
    printf(" )\n");
}

void give_name(void *object, const string &name) {
    object_names.insert({object, name});
}

void end_scope() {
    debug_scope--;
}

//Event *create_do_nothing() {
//    auto do_nothing = create(C Event([]() {
//        debug_print("do_nothing", {});
//        end_scope();
//    }), "do_nothing");
//
//    return do_nothing;
//}

void initialize_garbo() {
    finished_event->event = create(C Event([]() {}), "finished_event_internal");
    finished_event->next = finished_event;
    finished_event->prev = finished_event;
}

PropagatedEvent *new_default_propagated_event(Event *event) {
    auto propagated_event = create(C PropagatedEvent{
            .before = {},
            .after = {},
            .order = 0,
            .event = event
    }, name_of(event) + "_propagated_event");
    event_to_propagated_event.insert({event, propagated_event});
    return propagated_event;
}

void run_all_events() {
    debug_print("run_all_events", {});

    printf("Events to run: \n");
    print_sequenced_events();

    do {
//        printf("--- Events BEFORE: ");
//        print_sequenced_events();

//        printf("Running event seq: %s\n", name_of(current_sequenced_event).c_str());
        printf("Running event: %s\n", name_of(current_sequenced_event->event).c_str());
        (*current_sequenced_event->event)();

//        printf("--- Events AFTER: ");
//        print_sequenced_events();

        current_sequenced_event = current_sequenced_event->next;
    } while (current_sequenced_event != finished_event);

    end_scope();
}

void unschedule_event(Event *event) {
    debug_print("unschedule_event", {event});

//    printf("Events before: ");
//    print_sequenced_events();

    auto candidate = finished_event;
    while (true) {
        candidate = candidate->next;
//        printf("on: %s\n", +name_of(candidate->event).c_str());
//        printf("internal: %s\n", +name_of(event).c_str());
        if (candidate == finished_event) break;

        if (candidate->event != event) continue;

//        printf("FOUND: %s\n", +name_of(candidate).c_str());

        candidate->prev->next = candidate->next;
        candidate->next->prev = candidate->prev;

        if (candidate == current_sequenced_event) {
            // unscheduling current event requires move to next event
            current_sequenced_event = current_sequenced_event->next->prev; // prev on purpose
        }

        destroy(event);
        destroy(candidate);
        break;
    }

    printf("Events after: \n");
    print_sequenced_events();

    end_scope();
}

void schedule_recurring_event_after(SequencedEvent *reference, Event *event) {
    debug_print("schedule_recurring_event_after", {reference, event});

//    printf("Before schedule: ");
//    print_sequenced_events();

    auto sequenced_event = create(C SequencedEvent{
            .prev = reference,
            .next = reference->next,
            .event = event
    }, name_of(event) + "_seq");

    reference->next->prev = sequenced_event;
    reference->next = sequenced_event;

    printf("After schedule:\n");
    print_sequenced_events();

    end_scope();
}

void schedule_recurring_event_before(SequencedEvent *reference, Event *event) {
    schedule_recurring_event_after(reference->prev, event);
}

//void schedule_recurring_event_next(Event *event) {
//    schedule_recurring_event_after(current_sequenced_event, event);
//}

void schedule_recurring_event_last(Event *event) {
    schedule_recurring_event_before(finished_event, event);
}

Event *as_once(Event *event) {
    // TODO this is the most wack shit i have ever written

    // lambdas do not like deleting themselves
    // instead, create handle, assign after defining body, and delete deref of handle

    auto event_as_once_handle = create(C (Event *), name_of(event) + "_as_once_handle");

    auto event_as_once = create(C Event([=]() {
        auto event_as_once_internal = *event_as_once_handle;

        (*event)();
        destroy(event);

        unschedule_event(event_as_once_internal);

        // TODO this has to be freed, crashes rn
//        destroy(event_as_once_handle);
    }), name_of(event) + "_once");

    *event_as_once_handle = event_as_once;

    return event_as_once;
}

void schedule_recurring_event_using_order(int order, Event *event) {
    debug_print("schedule_recurring_event_using_order", {event});
    auto c = current_sequenced_event->next;
    while (true) {
        if (!event_to_propagated_event.contains(c->event)) break;

        auto propagated_c = event_to_propagated_event.at(c->event);
        if (order < propagated_c->order) break;

        c = c->next;
    }

    schedule_recurring_event_before(c, event);
    end_scope();
}

void increase_propagated_event_order(PropagatedEvent *propagated_event) {
    debug_print("increase_propagated_event_order", {propagated_event});

    int max_order = 0;
    for (auto before: propagated_event->before) {
        if (before->order > max_order) max_order = before->order;
    }

    int new_order = max_order + 1;
    if (new_order <= propagated_event->order) {
        end_scope();
        return;
    }

    propagated_event->order = new_order;
    for (auto after: propagated_event->after) {
        schedule_recurring_event_using_order(after->order, as_once(create(C Event([=]() {
            increase_propagated_event_order(after);
        }), "idk")));
    }

    end_scope();
}

void decrease_propagated_event_order(PropagatedEvent *propagated_event) {
    debug_print("decrease_propagated_event_order", {propagated_event});

    int max_order = 0;
    for (auto before: propagated_event->before) {
        if (before->order > max_order) max_order = before->order;
    }

    int new_order = max_order + 1;
    if (new_order >= propagated_event->order) {
        end_scope();
        return;
    }

    propagated_event->order = new_order;
    for (auto after: propagated_event->after) {
        schedule_recurring_event_using_order(after->order, as_once(create(C Event([=]() {
            decrease_propagated_event_order(after);
        }), "TEMP 3")));
    }

    end_scope();
}

void
register_propagated_event_order(PropagatedEvent *propagated_event_before, PropagatedEvent *propagated_event_after) {
    debug_print("register_propagated_event_order", {propagated_event_before, propagated_event_after});

    propagated_event_before->after.insert(propagated_event_after);
    propagated_event_after->before.insert(propagated_event_before);

    auto increase_order_event = as_once(create(C Event([=]() {
        debug_print("increase_order_event", {propagated_event_before, propagated_event_after});

        increase_propagated_event_order(propagated_event_after);

        end_scope();
    }), name_of(propagated_event_before) + "_leads_" + name_of(propagated_event_after)));

    schedule_recurring_event_last(increase_order_event);

    end_scope();
}

void
unregister_propagated_event_order(PropagatedEvent *propagated_event_before, PropagatedEvent *propagated_event_after) {
    debug_print("unregister_propagated_event_order", {propagated_event_before, propagated_event_after});

    propagated_event_before->after.erase(propagated_event_after);
    propagated_event_after->before.erase(propagated_event_before);

    schedule_recurring_event_last(as_once(create(C Event([=]() {
        debug_print("ANON5", {});

        decrease_propagated_event_order(propagated_event_after);

        end_scope();
    }), "TEMP 2")));

    end_scope();
}

void unregister_propagated_event(PropagatedEvent *propagated_event) {
    debug_print("unregister_propagated_event", {propagated_event});

    for (auto before: propagated_event->before) {
        unregister_propagated_event_order(before, propagated_event);
    }
    for (auto after: propagated_event->after) {
        unregister_propagated_event_order(propagated_event, after);
    }

    end_scope();
}

void schedule_propagated_event(PropagatedEvent *propagated_event) {
    debug_print("schedule_propagated_event", {propagated_event});

    auto trigger_and_schedule = as_once(create(C Event([=]() {
        debug_print("trigger_and_schedule", {propagated_event});

        (*propagated_event->event)();
        for (auto propagated_event_after: propagated_event->after) {
            schedule_propagated_event(propagated_event_after);
        }

        end_scope();
    }), name_of(propagated_event) + "_trig"));

    schedule_recurring_event_using_order(propagated_event->order, trigger_and_schedule);

    end_scope();
}

void destroy_object(int *object) {
    debug_print("destroy_object", {object});

    auto destroy_propagated_event = datum_to_destroy_propagated_event.at(object);
    schedule_propagated_event(destroy_propagated_event);

    run_all_events();

    end_scope();
}

void subsume_lifetime_under_lifetime(void *subsumed, void *dominant) {
    debug_print("subsume_lifetime_under_lifetime", {subsumed, dominant});

    auto destroy_lifetime_subsumed = datum_to_destroy_propagated_event.at(subsumed);
    auto destroy_lifetime_dominant = datum_to_destroy_propagated_event.at(dominant);

    register_propagated_event_order(destroy_lifetime_dominant, destroy_lifetime_subsumed);

    end_scope();
}

void provide_lifetime(void *object, Event *destroyer) {
    debug_print("provide_lifetime", {object, destroyer});

    auto destroy_propagated_event_handle = create(C PropagatedEvent *,
                                                  name_of(object) + "_destroy_propagated_event_handle");
    auto destroy_propagated_event = new_default_propagated_event(create(C Event([=]() {
        auto destroy_propagated_event_internal = *destroy_propagated_event_handle;

        debug_print("destroy_propagated_event", {object});

        (*destroyer)();
        destroy(destroyer);

        unregister_propagated_event(destroy_propagated_event_internal);

        end_scope();
    }), name_of(object) + "_destroy"));
    *destroy_propagated_event_handle = destroy_propagated_event;

    datum_to_destroy_propagated_event.insert({object, destroy_propagated_event});

    end_scope();
}

template<typename T>
void default_lifetime(T *object) {
    provide_lifetime(object, create(C Event([=]() {
        destroy(object);
    }), name_of(object) + "_destroyer"));
}

PropagatedEvent *
create_datum_update_propagated_event(int *datum, Event *datum_update_event, const unordered_set<int *> &dependencies) {
    debug_print("create_datum_update_propagated_event", {datum, datum_update_event});

    auto update_propagated_event = new_default_propagated_event(datum_update_event);
    default_lifetime(update_propagated_event);

    subsume_lifetime_under_lifetime(update_propagated_event, datum);

    for (auto dependency: dependencies) {
        auto dependency_update_propagated_event = datum_to_update_propagated_event.at(dependency);
        register_propagated_event_order(dependency_update_propagated_event, update_propagated_event);

        subsume_lifetime_under_lifetime(update_propagated_event, dependency);
    }

    schedule_propagated_event(update_propagated_event);

    run_all_events();

    end_scope();
    return update_propagated_event;
}

void assign_datum(int *datum, int value) {
    debug_print("assign_datum", {datum});

    *datum = value;
    schedule_propagated_event(datum_to_update_propagated_event.at(datum));

    run_all_events();

    end_scope();
}

void make_observed_datum(int *datum) {
    debug_print("make_observed_datum", {datum});

    default_lifetime(datum);

    auto update_propagated_event = new_default_propagated_event(
            create(C Event([]() {}), name_of(datum) + "_update"));
    default_lifetime(update_propagated_event); // got a feeling this should not be default lifetime (unregister?)

    datum_to_update_propagated_event.insert({datum, update_propagated_event});

    end_scope();
}

void ugh_test() {
    debug_print("ugh_test", {});

    initialize_garbo();

    int *a = create(C int, "a");
    int *b = create(C int, "b");
    int *c = create(C int, "c");

    make_observed_datum(a);
    make_observed_datum(b);
    make_observed_datum(c);

    create_datum_update_propagated_event(c, create(C Event([=]() {
        debug_print("c_datum_update_event", {});
        *c = *a + *b;
        end_scope();
    }), "c_update"), {a, b});


    // datum_prop_event made for a and b
    // c datum_prop_event subsumes a and b
    // binding lifetime subsumes c datum_prop_event
    // first trigger of update event made

    printf("C: %d\n", *c);
    assign_datum(a, 2);
    printf("C: %d\n", *c);
    assign_datum(b, 4);
    printf("C: %d\n", *c);

    destroy_object(a);

    // c should live
    // x should die

    end_scope();
}