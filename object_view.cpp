#include "object_view.h"

ObjectView *selected_object_view = nullptr;

vector<ObjectViewBuilder> object_view_builders;

map<void *, ObjectType> object_to_type;
map<void *, Shared<Signal> *> object_to_signal;

unordered_map<void *, ObjectView *> object_to_view;
unordered_map<string, void **> name_to_object_handle;

void init_object_view_builders() {
    object_view_builders.push_back(none_object_view_builder);
    object_view_builders.push_back(do_then_object_view_builder);
    object_view_builders.push_back(add_object_view_builder);
    object_view_builders.push_back(assign_object_view_builder);
    object_view_builders.push_back(run_object_view_builder);
    object_view_builders.push_back(integer_object_view_builder);
    object_view_builders.push_back(string_object_view_builder);
    object_view_builders.push_back(declare_object_view_builder);
    object_view_builders.push_back(if_object_view_builder);
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

void include_sub_box(ObjectView *o, Box* sub_box, Signal *sub_box_sig) {
    o->sub_boxes.push_back(sub_box);

    o->internal_constraints.push_back(create_listener({sub_box_sig}, new function<void(void)>([=]() {
        Box large_box = {FLT_MAX, -FLT_MAX, FLT_MAX, -FLT_MAX};
        for (auto s: o->sub_boxes) {
            large_box = enclosing_box(large_box, *s);
        }
        o->box = large_box;
        signal_update(&o->box_sig);
    })));
}

void include_sub_object_view(ObjectView *object_view, ObjectView *sub_object_view) {
    object_view->sub_object_views.push_back(sub_object_view);
    include_sub_box(object_view, &sub_object_view->box, &sub_object_view->box_sig);
}

void redo_sub_objects(ObjectView *o, const ObjectViewBuilder &object_view_builder) {
    o->previous_destroy_sub_object_views(o);
    object_to_view.erase({ *o->object_handle});

    o->previous_destroy_sub_object_views = object_view_builder.destroy_sub_object_views;
    o->editable_text.text = object_view_builder.s;
    signal_update(&o->editable_text.text_sig);

    object_view_builder.create_sub_object_views(o);
    if (*o->object_handle != nullptr) {
        object_to_view.insert({ *o->object_handle, o});
    }
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
    object_view->sub_boxes.clear();
}

ObjectView *new_object_view(void **object_handle) {
    auto o = new ObjectView;
    o->object_handle = object_handle;
    initialize_editable_text(&o->editable_text);
    o->previous_destroy_sub_object_views = find_object_view_builder(*o->object_handle).destroy_sub_object_views;

    include_sub_box(o, &o->editable_text.box, &o->editable_text.box_sig);

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

    return o;
}

void quick_layout_right(ObjectView *left, ObjectView* right, Box *o_box, Signal *o_box_sig, Box *s_editable_text_box, Signal *s_editable_text_box_sig) {
    left->sub_object_constraints.push_back(create_listener({o_box_sig}, new function<void(void)>([=]() {
        box_layout_right(o_box, s_editable_text_box);
        signal_update(s_editable_text_box_sig);
    })));
    include_sub_object_view(left, right);
}