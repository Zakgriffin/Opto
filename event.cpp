#include "event.h"

#define DEFAULT_LIFETIME(x) provide_lifetime(x, new Event([=]() { delete x; }))

int debug_scope = 0;

map<void *, string> object_names;

struct PropagatedEvent {
    Event *event;
    unordered_set<PropagatedEvent *> before;
    unordered_set<PropagatedEvent *> after;
    int order;
};

SequencedEvent *finished_event = new SequencedEvent;


SequencedEvent *current_sequenced_event = finished_event;

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
        printf(" -> %s", name_of(s).c_str());
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

Event *do_nothing = new Event([]() {
    debug_print("do_nothing", {});
    end_scope();
});

void initialize_garbo() {
    finished_event->event = do_nothing;
    finished_event->next = finished_event;
    finished_event->prev = finished_event;

    give_name(finished_event, "finished_event");
}

PropagatedEvent *new_default_propagated_event(Event *event) {
    return new PropagatedEvent{
            .event = event,
            .before = {},
            .after = {},
            .order = 0
    };
}

void run_all_events() {
    debug_print("run_all_events", {});

    printf("Events to run: ");
    print_sequenced_events();

    while (true) {
        current_sequenced_event = current_sequenced_event->next;
        if (current_sequenced_event == finished_event) break;

        printf("Running event seq: %s\n", name_of(current_sequenced_event).c_str());
//        printf("Running event: %s\n", name_of(current_sequenced_event->event).c_str());
        (*current_sequenced_event->event)();

//        printf("Events remaining: ");
//        print_sequenced_events();
    }

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
            current_sequenced_event = current_sequenced_event->next;
        }

        delete event;
        delete candidate;
        break;
    }

    printf("Events after: ");
    print_sequenced_events();

    end_scope();
}

void schedule_recurring_event_next(Event *event) {
    debug_print("schedule_recurring_event_next", {event});

//    printf("Before schedule: ");
//    print_sequenced_events();

    auto sequenced_event = new SequencedEvent{
            .event = event,
            .prev = current_sequenced_event,
            .next = current_sequenced_event->next
    };
    give_name(sequenced_event, name_of(event) + "_seq");

    current_sequenced_event->next->prev = sequenced_event;
    current_sequenced_event->next = sequenced_event;

//    printf("After schedule: ");
//    print_sequenced_events();

    end_scope();
}

void schedule_recurring_event_last(Event *event) {
    debug_print("schedule_recurring_event_last", {event});

    auto sequenced_event = new SequencedEvent{
            .event = event,
            .prev = current_sequenced_event->prev,
            .next = current_sequenced_event
    };
    give_name(sequenced_event, name_of(event) + "_seq");

    current_sequenced_event->prev->next = sequenced_event;
    current_sequenced_event->prev = sequenced_event;

    end_scope();
}

Event *as_once(Event *event) {
    // TODO this is the most wack shit i have ever written

    // lambdas do not like deleting themselves
    // instead, create handle, assign after defining body, and delete deref of handle

    auto event_as_once_handle = new (Event *);

    auto event_as_once = new Event([=]() {
        auto event_as_once_internal = *event_as_once_handle;

        (*event)();
        delete event;

        unschedule_event(event_as_once_internal);

        delete event_as_once_handle;
    });

    *event_as_once_handle = event_as_once;

    return event_as_once;
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
        schedule_recurring_event_last(as_once(new Event([=]() {
            debug_print("ANON2", {});

            increase_propagated_event_order(after);

            end_scope();
        })));
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
        schedule_recurring_event_last(as_once(new Event([=]() {
            debug_print("ANON3", {});

            increase_propagated_event_order(after);

            end_scope();
        })));
    }

    end_scope();
}

void
register_propagated_event_order(PropagatedEvent *propagated_event_before, PropagatedEvent *propagated_event_after) {
    debug_print("register_propagated_event_order", {propagated_event_before, propagated_event_after});

    propagated_event_before->after.insert(propagated_event_after);
    propagated_event_after->before.insert(propagated_event_before);

    auto increase_order_event = as_once(new Event([=]() {
        debug_print("increase_order_event", {propagated_event_before, propagated_event_after});

        increase_propagated_event_order(propagated_event_after);

        end_scope();
    }));
    give_name(increase_order_event, name_of(propagated_event_before) + "_leads_" + name_of(propagated_event_after));

    schedule_recurring_event_last(increase_order_event);

    end_scope();
}

void
unregister_propagated_event_order(PropagatedEvent *propagated_event_before, PropagatedEvent *propagated_event_after) {
    debug_print("unregister_propagated_event_order", {propagated_event_before, propagated_event_after});

    propagated_event_before->after.erase(propagated_event_after);
    propagated_event_after->before.erase(propagated_event_before);

    schedule_recurring_event_last(as_once(new Event([=]() {
        debug_print("ANON5", {});

        decrease_propagated_event_order(propagated_event_after);

        end_scope();
    })));

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

void schedule_propagated_event_next(PropagatedEvent *propagated_event) {
    debug_print("schedule_propagated_event_next", {propagated_event});

    auto trigger_and_schedule_next = as_once(new Event([=]() {
        debug_print("trigger_and_schedule_next", {});

        (*propagated_event->event)();
        for (auto propagated_event_after: propagated_event->after) {
            schedule_propagated_event_next(propagated_event_after);
        }

        end_scope();
    }));
    give_name(trigger_and_schedule_next, name_of(propagated_event) + "_trig");

    schedule_recurring_event_last(trigger_and_schedule_next);

    end_scope();
}

void destroy_object(int *object) {
    debug_print("destroy_object", {object});

    auto destroy_propagated_event = datum_to_destroy_propagated_event.at(object);
    schedule_propagated_event_next(destroy_propagated_event);

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
    give_name(destroyer, name_of(object) + "_destroyer");
    debug_print("provide_lifetime", {object, destroyer});

    PropagatedEvent *destroy_propagated_event = new_default_propagated_event(new Event([=]() {
        debug_print("ANON7", {});

        (*destroyer)();
        delete destroyer;

        unregister_propagated_event(destroy_propagated_event);

        end_scope();
    }));
    give_name(destroy_propagated_event, name_of(object) + "_destroy_propagated_event");

    datum_to_destroy_propagated_event.insert({object, destroy_propagated_event});

    end_scope();
}

PropagatedEvent *
create_datum_update_propagated_event(int *datum, Event *datum_update_event, const unordered_set<int *> &dependencies) {
    give_name(datum_update_event, name_of(datum) + "_datum_update_event");
    debug_print("create_datum_update_propagated_event", {datum, datum_update_event});

    auto update_propagated_event = new_default_propagated_event(datum_update_event);
    give_name(update_propagated_event, name_of(datum) + "_update_propagated_event");
    DEFAULT_LIFETIME(update_propagated_event);

    subsume_lifetime_under_lifetime(update_propagated_event, datum);

    for (auto dependency: dependencies) {
        auto dependency_update_propagated_event = datum_to_update_propagated_event.at(dependency);
        register_propagated_event_order(dependency_update_propagated_event, update_propagated_event);
    }

    schedule_propagated_event_next(update_propagated_event);

    run_all_events();

    end_scope();
    return update_propagated_event;
}

void assign_datum(int *datum, int value) {
    debug_print("assign_datum", {datum});

    *datum = value;
    schedule_propagated_event_next(datum_to_update_propagated_event.at(datum));

    end_scope();
}

void make_observed_datum(int *datum) {
    debug_print("make_observed_datum", {datum});

    DEFAULT_LIFETIME(datum);

    auto update_propagated_event = new_default_propagated_event(do_nothing);
    give_name(update_propagated_event, name_of(datum) + "_update_propagated_event");
    DEFAULT_LIFETIME(update_propagated_event); // got a feeling this should not be default lifetime (unregister?)

    datum_to_update_propagated_event.insert({datum, update_propagated_event});

    end_scope();
}

void ugh_test() {
    debug_print("ugh_test", {});

    initialize_garbo();

    int *a = new int;
    give_name(a, "a");
    int *b = new int;
    give_name(b, "b");
    int *c = new int;
    give_name(c, "c");

    make_observed_datum(a);
    make_observed_datum(b);
    make_observed_datum(c);

    create_datum_update_propagated_event(c, new Event([=]() {
        debug_print("update_c", {});
        *c = *a + *b;
        end_scope();
    }), {a, b});

    // datum_prop_event made for a and b
    // c datum_prop_event subsumes a and b
    // binding lifetime subsumes c datum_prop_event
    // first trigger of update event made

    assign_datum(a, 2);
    assign_datum(b, 4);

    destroy_object(a);

    // c should live
    // x should die

    end_scope();
}