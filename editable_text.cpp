#include "editable_text.h"

EditableText *selected_editable_text = nullptr;

int PAD_X = 5;
int PAD_Y = 3;

float CHARACTER_WIDTH = 8;

float b(EditableText *e, int character_index) {
    return e->box.x_min + CHARACTER_WIDTH * character_index + PAD_X;
}

void draw_editable_text(EditableText *e) {
    Rectangle rect = box_to_rectangle(e->box);
    DrawRectangleRec(rect, e->color);
    DrawTextEx(font, e->text.c_str(), Vector2{e->box.x_min + PAD_X, e->box.y_min + PAD_Y}, (float) font.baseSize / 2, 0, WHITE);
    if (selected_editable_text == e) {
        float k = b(e, e->character_index);
        DrawLine(k, e->box.y_min, k, e->box.y_max, WHITE);
    }
}

void finalize_editable_text(EditableText *e) {
    for (const auto &l: e->internal_listeners) {
        destroy_listener(l);
    }
}

void initialize_editable_text(EditableText *e) {
    e->text = "";
    e->character_index = 0;
    e->box = Box{};
    e->color = BOX_COLOR;

    e->internal_listeners.push_back(create_listener({&e->text_sig}, new function<void(void)>([=]() {
        e->box.x_max = e->box.x_min + CHARACTER_WIDTH * e->text.size() + 2 * PAD_X;
        e->box.y_max = e->box.y_min + (float) font.baseSize / 2 + 2 * PAD_Y;
        signal_update(&e->box_sig);
    })));

    e->internal_listeners.push_back(create_listener({&input_listeners}, new function<void(void)>([=]() {
        signal_update(&e->key_pressed_sig);
    })));

    e->internal_listeners.push_back(create_listener({&e->key_pressed_sig}, new function<void(void)>([=]() {
        if (edit_mode != EDITABLE_TEXT) return;

        Vector2 mouse = GetMousePosition();
        if (is_within_box(mouse, e->box)) {
            mouse_cursor = 2;
            if (IsMouseButtonPressed(0)) {
                selected_editable_text = e;
                auto end = e->text.size();
                auto clicked_character_index= (int) map_range<float>(mouse.x, b(e, 0), b(e, end), 0.5, end + 0.5);
                if(clicked_character_index > end) clicked_character_index = end;
                e->character_index = clicked_character_index;
                mouse_clicked_during_input = true;
            }
        }

        if (selected_editable_text != e) return;

        if (key_pressed != 0) {
            if (key_pressed == KEY_BACKSPACE) {
                if (e->text.length() > 0 && e->character_index > 0) {
                    e->character_index--;
                    e->text.erase(e->character_index, 1);
                    signal_update(&e->text_sig);
                }
            } else if (key_pressed == KEY_LEFT) {
                if (e->character_index > 0) e->character_index--;
            } else if (key_pressed == KEY_RIGHT) {
                if (e->character_index < e->text.size()) e->character_index++;
            } else if (key_pressed == KEY_ESCAPE) {
                edit_mode = OBJECT_VIEW;
                selected_editable_text = nullptr;
            } else {
                auto c = (char) tolower(key_pressed);
                e->text.insert(e->character_index, 1, c);
                e->character_index++;
                signal_update(&e->text_sig);
            }
        }
    })));


//    create_binding({text}, new Event([&](){
//        box.width = 123;
//        box.height = 321;
//    }));
//
//    auto bool_is_true_lifetime = x;
//    create_datum_update_propagated_event(new Event([]() {
//        if(selected) {
//            for (char key = ' '; key <= '~'; key++) {
//                char lower = (char) tolower(key);
//                auto listener = create_key_pressed_listener(key, new Event([&, lower]() {
//                    text.insert(character_index, 1, lower);
//                    character_index++;
//                }));
//                lifetime_subsumes(listener, bool_is_true_lifetime);
//            }
//        } else {
//            destroy_object(bool_is_true_lifetime);
//        }
//    }, {selected}));

    e->internal_listeners.push_back(create_listener({&draw_visuals}, new function<void(void)>([=]() {
        draw_editable_text(e);
    })));
}