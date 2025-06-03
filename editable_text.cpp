#include "editable_text.h"

EditableText *selected_editable_text = nullptr;

int PAD_X = 5;
int PAD_Y = 3;

float bunk(int i) {
    return i * 8;
}

void draw_editable_text(EditableText *e) {
    DrawRectangleRec(e->box, e->color);
    DrawTextEx(font, e->text.c_str(), Vector2{e->box.x + PAD_X, e->box.y + PAD_Y}, (float) font.baseSize / 2, 0, WHITE);
    if (selected_editable_text == e) {
        int k = e->box.x + bunk(e->character_index) + PAD_X;
        DrawLine(k, e->box.y, k, e->box.y + e->box.height, WHITE);
    }
}

void finalize_editable_text(EditableText *e) {
    for(const auto& l : e->internal_listeners) {
        destroy_listener(l);
    }
}

void initialize_editable_text(EditableText *e) {
    e->text = "";
    e->character_index = 0;
    e->box = Rectangle{};
    e->color = BOX_COLOR;

    e->internal_listeners.push_back(create_listener({&e->text_sig}, new function<void(void)>([=]() {
        e->box.width = bunk(e->text.size()) + 2 * PAD_X;
        e->box.height = (float) font.baseSize / 2 + 2 * PAD_Y;
        signal_update(&e->size_sig);
    })));

    e->internal_listeners.push_back(create_listener({&input_listeners}, new function<void(void)>([=]() {
        if (edit_mode != EDITABLE_TEXT) return;

        Vector2 mouse = GetMousePosition();
        if (is_within_rect(mouse, &e->box)) {
            mouse_cursor = 2;
            if (IsMouseButtonPressed(0)) {
                selected_editable_text = e;
                e->character_index = 0;
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