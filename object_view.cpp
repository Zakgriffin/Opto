#include "object_view.h"

ObjectView *selected_object_view = nullptr;

void box_touch_left_right(Rectangle *left_box, Rectangle *right_box) {
    right_box->x = left_box->x + left_box->width;
}

void box_touch_up_down(Rectangle *up_box, Rectangle *down_box) {
    down_box->y = up_box->y + up_box->height;
}

void box_align_left(Rectangle *box_1, Rectangle *box_2) {
    box_2->x = box_1->x;
}

void box_align_up(Rectangle *box_1, Rectangle *box_2) {
    box_2->y = box_1->y;
}

map<string, UnknownConverter> unknown_converters;

void init_unknown_converters() {
    unknown_converters.insert({"do-then", UnknownConverter{DO_THEN, do_then_to_unknown, do_then_from_unknown, do_then_create_sub_object_views, do_then_destroy_sub_object_views}});
    unknown_converters.insert({"string", UnknownConverter{STRING, string_to_unknown, string_from_unknown, string_create_sub_object_views, string_destroy_sub_object_views}});
}

Vector2 mouse_offset;

void destroy_object_view(ObjectView *object_view) {
    for (const auto& x: object_view->internal_constraints) {
        destroy_listener(x);
    }
    finalize_editable_text(&object_view->editable_text);
    // ZZZ not done

    delete object_view;
}

void nothing_destroy_sub_object_views(ObjectView *object_view) {
}

ObjectView *new_object_view(void **object_handle, ObjectType object_type) {
    auto o = new ObjectView;
    o->object_type = object_type;
    o->object_handle = object_handle;
    initialize_editable_text(&o->editable_text);
    selected_editable_text = &o->editable_text;
    edit_mode = EDITABLE_TEXT;
    o->box = Rectangle{};
    o->previous_destroy_sub_object_views = nothing_destroy_sub_object_views;

    o->internal_constraints.push_back(create_listener({&o->position_sig}, new function<void(void)>([=]() {
        o->editable_text.box.x = o->box.x;
        o->editable_text.box.y = o->box.y;
    })));

    o->internal_constraints.push_back(create_listener({&o->editable_text.size_sig}, new function<void(void)>([=]() {
        o->box.width = o->editable_text.box.width;
        o->box.height = o->editable_text.box.height;
        signal_update(&o->size_sig);
    })));

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
        if (is_within_rect(mouse, &o->box)) {
            if (IsMouseButtonPressed(0)) {
                selected_object_view = o;
                mouse_clicked_during_input = true;
                mouse_offset.x = mouse.x - o->box.x;
                mouse_offset.y = mouse.y - o->box.y;
            }

            auto click_streak = check_clicked_n_times(&o->multi_click, 2);
            if (click_streak == 2) {
                selected_editable_text = &o->editable_text;
                selected_object_view = nullptr;
                edit_mode = EDITABLE_TEXT;
            }
            if (click_streak >= 1) mouse_clicked_during_input = true;
        }

        if (selected_object_view == o) {
            if (IsMouseButtonReleased(0)) {
                selected_object_view = nullptr;
            } else {
                o->box.x = mouse.x - mouse_offset.x;
                o->box.y = mouse.y - mouse_offset.y;
                signal_update(&o->position_sig);
            }
        }
    })));

    o->internal_constraints.push_back(create_listener({&draw_visuals}, new function<void(void)>([=]() {
        if (o == selected_object_view) {
            float PAD = 6;
            auto surround = Rectangle{
                    .x=o->box.x - PAD,
                    .y=o->box.y - PAD,
                    .width=o->box.width + PAD * 2,
                    .height=o->box.height + PAD * 2
            };
            DrawRectangleRec(surround, Color(255, 255, 255, 80));
        }

        for (auto i = 0; i < o->potential_lookup.size(); i++) {
            auto p = o->potential_lookup.at(i);
            DrawTextEx(font, p.c_str(), Vector2{o->box.x, o->box.y + o->box.height + 12 * i}, font.baseSize / 2, 0, GRAY);
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