#include "editable_text.h"

EditableText *selected_editable_text = nullptr;
unordered_set<EditableText*> editable_texts;

int PAD_X = 5;
int PAD_Y = 3;

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
    editable_texts.erase(e);
}

int x_to_character_index(EditableText *e, float x) {
    auto end = (int) e->text.size();
    auto mapped = map_range<float>(x, b(e, 0), b(e, end), 0.5, end + 0.5);
    return clamp((int) round(mapped), 0, end);
}

int character_index_to_x(EditableText *e, int i) {
    auto end = (int) e->text.size();
    auto mapped = map_range<float>(i,0.5, end + 0.5, b(e, 0), b(e, end));
    return mapped;
}

void jump_editable_text(EditableText *e, float Box::* low, float Box::* high, float flip) {
    EditableText* best = nullptr;
    auto b = e->box;
    b.*low = b.*high;
    b.*high = MAXFLOAT * flip;

    for (auto et : editable_texts) {
        if (!boxes_overlap(et->box, b)) continue;
        if (best == nullptr || (et->box.*low - best->box.*low)*flip < 0) best = et;
    }
    if (!best) return;

    selected_editable_text = best;
    auto x = character_index_to_x(e, e->character_index);
    best->character_index = x_to_character_index(best, x);
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

    e->internal_listeners.push_back(create_listener({&e->text_input_sig}, new function<void(void)>([=]() {
        signal_update(&e->text_sig);
    })));

    e->internal_listeners.push_back(create_listener({&e->key_pressed_sig}, new function<void(void)>([=]() {
        if (edit_mode != EDITABLE_TEXT) return;

        Vector2 mouse = GetMousePosition();
        if (is_within_box(mouse, e->box)) {
            mouse_cursor = 2;
            if (IsMouseButtonPressed(0)) {
                selected_editable_text = e;
                auto end = e->text.size();
                auto clicked_character_index = x_to_character_index(e, mouse.x);
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
                    signal_update(&e->text_input_sig);
                }
            } else if (key_pressed == KEY_LEFT) {
                if (e->character_index > 0) e->character_index--;
                else jump_editable_text(e, &Box::x_max, &Box::x_min, -1);
            } else if (key_pressed == KEY_RIGHT) {
                if (e->character_index < e->text.size()) e->character_index++;
                else jump_editable_text(e, &Box::x_min, &Box::x_max, 1);
            } else if (key_pressed == KEY_UP) {
                jump_editable_text(e, &Box::y_max, &Box::y_min, -1);
            } else if (key_pressed == KEY_DOWN) {
                jump_editable_text(e, &Box::y_min, &Box::y_max, 1);
            } else if (key_pressed == KEY_SPACE) {
                jump_editable_text(e, &Box::x_min, &Box::x_max, 1);
            } else if (key_pressed == KEY_ESCAPE) {
                edit_mode = OBJECT_VIEW;
                selected_editable_text = nullptr;
            } else if (key_pressed == KEY_BACKSPACE) {
                // collapse_object_view(o);
            } else {
                auto c = (char) tolower(key_pressed);
                e->text.insert(e->character_index, 1, c);
                e->character_index++;
                signal_update(&e->text_input_sig);
            }
        }
    })));

    e->internal_listeners.push_back(create_listener({&draw_visuals}, new function<void(void)>([=]() {
        draw_editable_text(e);
    })));

    editable_texts.insert(e);
}