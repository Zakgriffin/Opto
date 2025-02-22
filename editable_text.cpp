#include "editable_text.h"

EditableText::EditableText() {
    text = "";
    character_index = 0;
    selected = false;
    /*
    create_binding({text}, new Event([&](){
        box.width = 123;
        box.height = 321;
    }));

    auto bool_is_true_lifetime = x;
    create_datum_update_propagated_event(new Event([]() {
        if(selected) {
            for (char key = ' '; key <= '~'; key++) {
                char lower = (char) tolower(key);
                auto listener = create_key_pressed_listener(key, new Event([&, lower]() {
                    text.insert(character_index, 1, lower);
                    character_index++;
                }));
                lifetime_subsumes(listener, bool_is_true_lifetime);
            }
        } else {
            destroy_object(bool_is_true_lifetime);
        }
    }, {selected}));

    visuals.insert(this);
     */
}

void EditableText::draw() {
    DrawTextEx(font, text.c_str(), Vector2{box.x, box.y}, (float) font.baseSize / 2, 0, WHITE);
}