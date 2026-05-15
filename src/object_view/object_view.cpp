#include "globals.h"
#include "none.h"
#include "object_view.h"

ObjectView *selected_object_view = nullptr;

vector<ObjectViewBuilder> object_view_builders;

unordered_map<void *, ObjectType> object_to_type;
unordered_map<void *, Shared<Signal> *> object_to_signal;

unordered_set<ObjectView *> object_views;
unordered_map<void*, string> object_to_name;

unordered_map<void *, ObjectView *> object_to_view;

ray::Vector2 mouse_offset;

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

    object_views.erase(object_view);
    delete object_view;
}

void include_sub_box(ObjectView *o, Box* sub_box, Signal *sub_box_sig) {
    o->sub_boxes.push_back(sub_box);
    sub_box_sig->on_change.insert(o->sub_box_listener.f);
    signal_update(sub_box_sig);
}

ObjectViewBuilder find_object_view_builder(void *object)
{
    auto type = object_to_type.at(object);
    for (auto object_view_builder : object_view_builders) {
        if (object_view_builder.object_type == type)
        {
            return object_view_builder;
        }
    }

    cout << "ZZZZ bad object no builder" << endl;
    return none_object_view_builder;
}

void include_sub_object_view(ObjectView *object_view, ObjectView *sub_object_view) {
    object_view->sub_object_views.push_back(sub_object_view);
    include_sub_box(object_view, &sub_object_view->box, &sub_object_view->box_sig);
}

void collapse_sub_objects(ObjectView *o) {
    o->destroy_sub_object_views_previous(o);
    o->destroy_sub_object_views_previous = none_destroy_sub_object_views;

    if (*o->object_handle != nullptr) {
        // cout << "ERASE!!" << endl;
        object_to_view.erase({ *o->object_handle});
    }

    auto text = object_to_name.contains(*o->object_handle) ? object_to_name.at(*o->object_handle) : "?";
    o->editable_text.text = text;
    o->editable_text.color = COLLAPSED_COLOR;

    signal_update(&o->editable_text.text_sig);

    o->collapsed = true;
}

void expand_sub_objects(ObjectView *o) {
    auto object_view_builder = find_object_view_builder(*o->object_handle);
    o->destroy_sub_object_views_previous = object_view_builder.destroy_sub_object_views;

    o->editable_text.text = object_view_builder.s;
    o->editable_text.color = BOX_COLOR;
    signal_update(&o->editable_text.text_sig);

    object_view_builder.create_sub_object_views(o);

    if (*o->object_handle != nullptr) {
        // cout << "INSERT!!" << endl;
        object_to_view.insert({ *o->object_handle, o});
    }

    o->collapsed = false;
}

void redo_sub_objects(ObjectView *o) {
    o->destroy_sub_object_views_previous(o);
    o->destroy_sub_object_views_previous = none_destroy_sub_object_views;

    if (*o->object_handle == nullptr || !object_to_name.contains(*o->object_handle)) {
        // && !object_to_view.contains(*o->object_handle)

        expand_sub_objects(o);
    } else {
        collapse_sub_objects(o);
    }
}

void generic_destroy_sub_object_views(ObjectView *object_view) {
    for (const auto &sub_object_view: object_view->sub_object_views) {
        sub_object_view->destroy_sub_object_views_previous(sub_object_view); // recurses
        destroy_object_view(sub_object_view);
    }

    for (const auto &sub_object_constraint: object_view->sub_object_constraints) {
        destroy_listener(sub_object_constraint);
    }

    object_view->sub_object_constraints.clear();
    object_view->sub_object_views.clear();
    object_view->sub_boxes.clear();

    object_view->sub_boxes.push_back(&object_view->editable_text.box);
    signal_update(&object_view->box_sig);
}

void gump(ObjectView *o, void* object) {
    *o->object_handle = object;
    signal_update(&object_to_signal.at(o->object_handle)->o);
}

ObjectView *new_object_view(void **object_handle) {
    auto o = new ObjectView;

    o->object_handle = object_handle;
    o->context = nullptr;
    initialize_editable_text(&o->editable_text);

    o->destroy_sub_object_views_previous = find_object_view_builder(*o->object_handle).destroy_sub_object_views;

    object_views.insert(o);

    o->sub_box_listener = create_listener({}, new function<void(void)>([=]() {
        Box large_box = {FLT_MAX, -FLT_MAX, FLT_MAX, -FLT_MAX};
        for (auto s: o->sub_boxes) {
            large_box = enclosing_box(large_box, *s);
        }
        o->box = large_box;
        signal_update(&o->box_sig);
    }));
    o->internal_constraints.push_back(o->sub_box_listener);
    include_sub_box(o, &o->editable_text.box, &o->editable_text.box_sig);

    o->internal_constraints.push_back(create_listener({lift_reference(o->object_handle)}, new function<void(void)>([=]() {
        redo_sub_objects(o);

        // if (type_check(*o->object_handle)) {
        //     o->editable_text.underline_color = GREEN;
        // } else {
        //     o->editable_text.underline_color = RED;
        // }
    })));

    o->internal_constraints.push_back(create_listener_lazy({&o->editable_text.text_input_sig}, new function<void(void)>([=]() {
        if (ray::IsKeyPressed(ray::KEY_LEFT_ALT)) {
            // ZZZZ straight forcing all this now
            if (!object_to_name.contains(*o->object_handle)) {
                object_to_name[*o->object_handle] = "";
                if (ray::GetKeyPressed() == 0) return;
            }
            auto text = o->editable_text.text;
            object_to_name[*o->object_handle] = text;
            o->editable_text.text = text;
            cout << "named " << text << endl;
            return;
        }

        for (const auto &object_view_builder: object_view_builders) {
            if (object_view_builder.s == o->editable_text.text) {
                gump(o, object_view_builder.create_simple());
                break;
            }
        }

        for (auto [obj, name] : object_to_name) {
            if (name == o->editable_text.text) {
                gump(o, obj);
                break;
            }
        }
    })));

    o->internal_constraints.push_back(create_listener({&input_listeners}, new function<void(void)>([=]() {
        if (edit_mode != ASSEMBLE_MODE) return;

        auto mouse = ray::GetMousePosition();
        if (is_within_box(mouse, o->box)) {
            bool hovering_child = false; // ZZZZ sorta naive
            for (auto k: o->sub_object_views) {
                if (is_within_box(mouse, k->box)) hovering_child = true;
            }
            if (!hovering_child) {
                if (ray::IsMouseButtonPressed(0)) {
                    selected_object_view = o;
                    mouse_clicked_during_input = true;
                    mouse_offset.x = mouse.x - o->editable_text.box.x_min;
                    mouse_offset.y = mouse.y - o->editable_text.box.y_min;
                }

                auto click_streak = check_clicked_n_times(&o->multi_click, 2);
                if (click_streak == 2) {
                    selected_editable_text = &o->editable_text;
                    o->editable_text.character_index = 0;
                    selected_object_view = nullptr;
                    edit_mode = TEXT_MODE;
                }
                if (click_streak >= 1) mouse_clicked_during_input = true;
            }
        }

        if (selected_object_view == o) {
            ObjectView* hovered_object = nullptr;
            for (auto object_view : object_views) {
                auto k = object_view->sub_object_views;
                for (auto sub : object_view->sub_object_views) {
                    if (sub == selected_object_view) continue;
                    if (is_within_box(mouse, sub->editable_text.box)){
                        // sub->editable_text.color = ray::GRAY;
                        hovered_object = sub;
                    }
                }
            }

            if (ray::IsMouseButtonReleased(0)) {
                if (hovered_object) {
                    *hovered_object->object_handle = *selected_object_view->object_handle;
                    signal_update(&object_to_signal.at(hovered_object->object_handle)->o);

                    selected_object_view->editable_text.box.x_min = 9999; // ZZZZ yeah i know
                    selected_object_view->editable_text.box.y_min = 9999;
                    selected_object_view->editable_text.box.x_max = 10000; // ZZZZ yeah i know
                    selected_object_view->editable_text.box.y_max = 10000;
                    signal_update(&selected_object_view->editable_text.box_sig);
                }
                selected_object_view = nullptr;
            } else {
                move_box_x(&o->editable_text.box, mouse.x - mouse_offset.x);
                move_box_y(&o->editable_text.box, mouse.y - mouse_offset.y);
                signal_update(&o->editable_text.box_sig);
            }

            if (!key_consumed) {
                if (key_pressed == ray::KEY_LEFT_CONTROL) {
                    if (selected_object_view && selected_object_view->collapsed) {
                        expand_sub_objects(selected_object_view);
                    } else {
                        collapse_sub_objects(selected_object_view);
                    }
                    key_consumed = true;
                    selected_object_view = nullptr;
                } else if (key_pressed == ray::KEY_BACKSPACE) {
                    selected_object_view->editable_text.box.x_min = 9999; // ZZZZ yeah i know
                    selected_object_view->editable_text.box.y_min = 9999;
                    collapse_sub_objects(selected_object_view);

                    key_consumed = true;
                    selected_object_view = nullptr;
                }
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
            DrawRectangleRec(surround_rect, ray::Color(255, 255, 255, 80));
        }

        for (auto i = 0; i < o->potential_lookup.size(); i++) {
            auto p = o->potential_lookup.at(i);
            DrawTextEx(font, p.c_str(), ray::Vector2{o->box.x_min, o->box.y_max + 12 * i}, font.baseSize / 2, 0, ray::GRAY);
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

void quick_layout_under(ObjectView *left, ObjectView* right, Box *o_box, Signal *o_box_sig, Box *s_editable_text_box, Signal *s_editable_text_box_sig) {
    left->sub_object_constraints.push_back(create_listener({o_box_sig}, new function<void(void)>([=]() {
        box_layout_under(o_box, s_editable_text_box);
        signal_update(s_editable_text_box_sig);
    })));
    include_sub_object_view(left, right);
}