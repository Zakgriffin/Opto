#include "object_view.h"

ObjectView *selected_object_view = nullptr;

map<string, UnknownConverter> unknown_converters;

#define init_unknown_converter_yuck(name, s) unknown_converters.insert({#s, UnknownConverter{name##_create_sub_object_views, name##_destroy_sub_object_views}})
#define init_unknown_converter(name) init_unknown_converter_yuck(name, name)

void init_unknown_converters() {
    init_unknown_converter_yuck(do_then, do-then);
    init_unknown_converter(string);
    init_unknown_converter(integer);
    init_unknown_converter(add);
}

Vector2 mouse_offset;

void destroy_object_view(ObjectView *object_view) {
    for (const auto &x: object_view->internal_constraints) {
        destroy_listener(x);
    }
    finalize_editable_text(&object_view->editable_text);
    // ZZZ not done

    delete object_view;
}

void nothing_destroy_sub_object_views(ObjectView *object_view) {
}

void j(ObjectView *o, Signal *sub_sig) {
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
//    object_view->bounding_listener.signals.push_back(&sub_object_view->box_sig);
    j(object_view, &sub_object_view->box_sig);
}

ObjectView *new_object_view(void **object_handle) {
    auto o = new ObjectView;
    o->object_handle = object_handle;
    initialize_editable_text(&o->editable_text);
    o->box = Box{};
    o->previous_destroy_sub_object_views = nothing_destroy_sub_object_views;

    o->ZZZZ_color = Color(255, 255, 255, 50);

    // ZZZZ (used to be) more will be added to internal vector
    j(o, &o->editable_text.box_sig);

    o->internal_constraints.push_back(create_listener({&o->editable_text.text_sig}, new function<void(void)>([=]() {
//        o->potential_lookup.clear();
//        if (!o->editable_text.text.empty()) {
//            for (const auto &x: unknown_converters) {
//                auto name = x.first;
//                if (name.starts_with(o->editable_text.text)) {
//                    o->potential_lookup.push_back(name);
//                }
//            }
//        }


        if (!unknown_converters.contains(o->editable_text.text)) return;
        auto unknown_converter = unknown_converters.at(o->editable_text.text);


        o->previous_destroy_sub_object_views(o);
        o->previous_destroy_sub_object_views = unknown_converter.destroy_sub_object_views;
        unknown_converter.create_sub_object_views(o);

//        if (o->editable_text.text == "do-then") {
//            o->object = new DoThen{.effect=nullptr, .next = nullptr};
//            do_then_create_sub_object_views(o);
//            printf("make new do-then\n");
//        } else if (o->editable_text.text == "string") {
//            o->object = new String_{.s=""};
//            string_create_sub_object_views(o);
//            printf("make new string\n");
//        }
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
//        if (unknown_converters.contains(text)) {
//            auto unknown_converter = unknown_converters.at(text);
//            Unknown *unknown = unknown_converter.to_unknown(object);
//
//            assign_datum(&o->object, unknown_converter.from_unknown(unknown));
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