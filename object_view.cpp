#include "object_view.h"

ObjectView *selected_object_view = nullptr;

vector<ObjectViewBuilder> object_view_builders;

map<void *, ObjectType> object_to_type;
map<void *, Shared<Signal> *> object_to_signal;

void init_object_view_builders() {
    object_view_builders.push_back(none_object_view_builder);
    object_view_builders.push_back(do_then_object_view_builder);
    object_view_builders.push_back(add_object_view_builder);
}

Vector2 mouse_offset;

Signal *lift_reference(void *object) {
    if (!object_to_signal.contains(object)) {
        object_to_signal.insert({object, new Shared<Signal>});
    }
    auto r = object_to_signal.at(object);
    r->count++;
    return &r->o;
}

void drop_reference(void *object) {
    if (!object_to_signal.contains(object)) {
        cout << "ZZZZ this is not good" << endl;
        return;
    }
    auto r = object_to_signal.at(object);
    r->count--;

    if (r->count == 0) {
        object_to_signal.erase(object);
        delete r;
    }
}

void destroy_object_view(ObjectView *object_view) {
    for (const auto &x: object_view->internal_constraints) {
        destroy_listener(x);
    }
    finalize_editable_text(&object_view->editable_text);
    drop_reference(object_view->object_handle);

    delete object_view;
}

void include_sub_box(ObjectView *o, Signal *sub_sig) {
    o->internal_constraints.push_back(create_listener({sub_sig}, new function<void(void)>([=]() {
        auto b = &o->editable_text.box; // known first element in collection
        Box large_box = {b->x_min, b->x_max, b->y_min, b->y_max};
        for (auto k: o->sub_object_views) {
            large_box = enclosing_box(large_box, k->box);
        }
        o->box = large_box;
        signal_update(&o->box_sig);
    })));
}

void include_sub_object_view(ObjectView *object_view, ObjectView *sub_object_view) {
    object_view->sub_object_views.push_back(sub_object_view);
    include_sub_box(object_view, &sub_object_view->box_sig);
}

void redo_sub_objects(ObjectView *o, const ObjectViewBuilder &object_view_builder) {
    o->previous_destroy_sub_object_views(o);
    o->previous_destroy_sub_object_views = object_view_builder.destroy_sub_object_views;
    o->editable_text.text = object_view_builder.s;
    signal_update(&o->editable_text.text_sig);
    object_view_builder.create_sub_object_views(o);
}

ObjectViewBuilder find_object_view_builder(void *object) {
    auto type = object_to_type.at(object);
    for (auto object_view_builder: object_view_builders) {
        if (object_view_builder.object_type == type) {
            return object_view_builder;
        }
    }

    cout << "ZZZZ shouldn't be possible" << endl;
    return {};
}

void generic_destroy_sub_object_views(ObjectView *object_view) {
    for (const auto &sub_object_view: object_view->sub_object_views) {
        destroy_object_view(sub_object_view);

        generic_destroy_sub_object_views(sub_object_view);
    }

    for (const auto &sub_object_constraint: object_view->sub_object_constraints) {
        destroy_listener(sub_object_constraint);
    }

    object_view->sub_object_constraints.clear();
    object_view->sub_object_views.clear();
}

ObjectView *new_object_view(void **object_handle) {
    auto o = new ObjectView;
    o->object_handle = object_handle;
    initialize_editable_text(&o->editable_text);
    o->previous_destroy_sub_object_views = find_object_view_builder(*o->object_handle).destroy_sub_object_views;

    include_sub_box(o, &o->editable_text.box_sig);

    o->internal_constraints.push_back(create_listener({lift_reference(o->object_handle)}, new function<void(void)>([=]() {
        redo_sub_objects(o, find_object_view_builder(*o->object_handle));
    })));

    o->internal_constraints.push_back(create_listener_lazy({&o->editable_text.text_input_sig}, new function<void(void)>([=]() {
        for (const auto &object_view_builder: object_view_builders) {
            if (object_view_builder.s == o->editable_text.text) {
                *o->object_handle = object_view_builder.create_simple();
                signal_update(&object_to_signal.at(o->object_handle)->o);
                break;
            }
        }
    })));

    o->internal_constraints.push_back(create_listener({&input_listeners}, new function<void(void)>([=]() {
        if (edit_mode != OBJECT_VIEW) return;

        auto mouse = GetMousePosition();
        if (is_within_box(mouse, o->box)) {
            bool hovering_child = false; // ZZZZ sorta naive
            for (auto k: o->sub_object_views) {
                if (is_within_box(mouse, k->box)) hovering_child = true;
            }
            if (!hovering_child) {
                if (IsMouseButtonPressed(0)) {
                    selected_object_view = o;
                    mouse_clicked_during_input = true;
                    mouse_offset.x = mouse.x - o->editable_text.box.x_min;
                    mouse_offset.y = mouse.y - o->editable_text.box.y_min;
                }

                auto click_streak = check_clicked_n_times(&o->multi_click, 2);
                if (click_streak == 2) {
                    selected_editable_text = &o->editable_text;
                    selected_object_view = nullptr;
                    edit_mode = EDITABLE_TEXT;
                }
                if (click_streak >= 1) mouse_clicked_during_input = true;
            }
        }

        if (selected_object_view == o) {
            if (IsMouseButtonReleased(0)) {
                selected_object_view = nullptr;
            } else {
                move_box_x(&o->editable_text.box, mouse.x - mouse_offset.x);
                move_box_y(&o->editable_text.box, mouse.y - mouse_offset.y);
                signal_update(&o->editable_text.box_sig);
            }
        }
    })));

    o->internal_constraints.push_back(create_listener({&draw_visuals}, new function<void(void)>([=]() {
        if (o == selected_object_view) {
            float PAD = 3;
            auto surround_box = Box{
                    .x_min=o->box.x_min - PAD,
                    .x_max=o->box.x_max + PAD,
                    .y_min=o->box.y_min - PAD,
                    .y_max=o->box.y_max + PAD,
            };
            auto surround_rect = box_to_rectangle(surround_box);
            DrawRectangleRec(surround_rect, Color(255, 255, 255, 80));
        }

        for (auto i = 0; i < o->potential_lookup.size(); i++) {
            auto p = o->potential_lookup.at(i);
            DrawTextEx(font, p.c_str(), Vector2{o->box.x_min, o->box.y_max + 12 * i}, font.baseSize / 2, 0, GRAY);
        }
    })));

//    create_datum_update_propagated_event(new Event([=]() {
//        create_sub_object_views(o);
//    }), {&o->object}, {&o->box.group});
//
//    create_datum_update_propagated_event(new Event([=]() {
//        string text = o->editable_text->text;
//        if (object_view_builders.contains(text)) {
//            auto object_view_builder = object_view_builders.at(text);
//            Unknown *unknown = object_view_builder.to_unknown(object);
//
//            assign_datum(&o->object, object_view_builder.from_unknown(unknown));
//        }
//    }), {&o->editable_text->text}, {&o->object});
// do lazy
// need to know whats affected second arg? not necessarily
// error if change occurs to datum not listed in second arg during update
    return o;
}

// position text box 1 wherever
// react to text box position 1 and text box size 1, drives view box 2
//