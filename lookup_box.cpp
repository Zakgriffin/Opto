#include "lookup_box.h"
#include "globals.h"
#include "user_input.h"
#include "controls.h"
#include "reactivity.h"

LookupBox::LookupBox() {
    on_lookup = nullptr;
    // state initialization
    text = "";
    character_index = 0;
    prompt = "";
    selected = false;

    // constants and constraints
    pad_x = 5;
    pad_y = 3;

    create_binding(&text_x, {&rect.x}, [&]() { text_x = rect.x + pad_x; });
    create_binding(&text_y, {&rect.y}, [&]() { text_y = rect.y + pad_y; });

    create_binding(&text_x_end, {&rect.x, &rect.width}, [&]() { text_x_end = rect.x + rect.width; });

    create_binding(&drawn_text, {&text, &prompt},
                   [&]() { drawn_text = text.empty() ? prompt : text; });

    create_binding(&rect.width, {&drawn_text},
                   [&]() { rect.width = font_width * (float) drawn_text.size() + pad_x * 2; });
    create_binding(&rect.height, {},
                   [&]() { rect.height = (float) font_height + pad_y * 2; });

    end_data_sync();

    // event listeners
    key_escape_listener = new Fn([&]() {
        unselect();
    });
    listener_visual_infos[key_escape_listener] = {BLUE, "unselect lookup box"};

    key_right_listener = new Fn([&]() {
        if (character_index < text.size()) character_index++;
    });
    listener_visual_infos[key_right_listener] = {BLUE, "move to next character"};

    key_left_listener = new Fn([&]() {
        if (character_index > 0) character_index--;
    });
    listener_visual_infos[key_left_listener] = {BLUE, "move to previous character"};

    key_backspace_listener = new Fn([&]() {
        if (character_index > 0) {
            text.erase(character_index - 1, 1);
            character_index--;
            on_text_change();
        }
    });
    listener_visual_infos[key_backspace_listener] = {BLUE, "delete character"};

    // TODO stack/priority based modifier keys
    key_left_super_listener = new Fn([&]() {
        destroy_key_pressed_listeners(key_listener_pairs);
        create_key_pressed_listeners(super_key_listeners);
        create_key_released_listener(KEY_LEFT_SUPER, release_super);
    });
    listener_visual_infos[key_left_super_listener] = {GetColor(0xA020F0FF), "super hotkeys"};

    key_left_alt_listener = new Fn([&]() {
        destroy_key_pressed_listeners(key_listener_pairs);
        create_key_pressed_listeners(alt_key_listeners);
        create_key_released_listener(KEY_LEFT_ALT, release_alt);
    });
    listener_visual_infos[key_left_alt_listener] = {GetColor(0xA020F0FF), "alt hotkeys"};

    key_listener_pairs = {
            {KEY_ESCAPE,     key_escape_listener},
            {KEY_RIGHT,      key_right_listener},
            {KEY_LEFT,       key_left_listener},
            {KEY_BACKSPACE,  key_backspace_listener},
            {KEY_LEFT_SUPER, key_left_super_listener},
            {KEY_LEFT_ALT,   key_left_alt_listener},
    };

    for (int key = 0x20; key <= 0x7E; key++) {
        char lower = (char) tolower(key);
        auto key_listener = new Fn([&, lower]() {
            text.insert(character_index, 1, (char) lower);
            character_index++;

            on_text_change();
        });
        key_listener_pairs.push_back({key, key_listener});


        string description = "type character '";
        description += lower;
        description += "'";

        listener_visual_infos[key_listener] = {GetColor(0xDD7700FF), description};
    }

    // super listeners
    super_key_a_listener = new Fn([&]() {

    });

    super_key_listeners = {
            {'A', super_key_a_listener},
    };

    // alt listeners

    alt_key_left_listener = new Fn([&]() {
        character_index = 0;
    });

    alt_key_right_listener = new Fn([&]() {
        character_index = (int) text.size();
    });

    alt_key_listeners = {
            {KEY_LEFT,  alt_key_left_listener},
            {KEY_RIGHT, alt_key_right_listener},
    };

    // release listeners

    release_super = new Fn([&]() {
        destroy_key_pressed_listeners(super_key_listeners);
        create_key_pressed_listeners(key_listener_pairs);
        destroy_key_released_listener(KEY_LEFT_SUPER, release_super);
    });

    release_alt = new Fn([&]() {
        destroy_key_pressed_listeners(alt_key_listeners);
        create_key_pressed_listeners(key_listener_pairs);
        destroy_key_released_listener(KEY_LEFT_ALT, release_alt);
    });

    // click listeners

    click_on_listener = new Fn([&]() {
        select();
    });

    click_off_listener = new Fn([&]() {
        unselect();
    });

    // hover handlers

    hover_handler = new HoverHandler{
            .is_within=within_rectangle(&rect),
            .hover_enter=[&]() {
                create_click_listener(click_on_listener);
                destroy_click_listener(click_off_listener);
                SetMouseCursor(MOUSE_CURSOR_IBEAM);
            },
            .hover_exit=[&]() {
                destroy_click_listener(click_on_listener);
                create_click_listener(click_off_listener);
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }
    };

    create_hover_handler(hover_handler);

    visuals.insert(this);
}

LookupBox::~LookupBox() {
    visuals.erase(this);

    destroy_hover_handler(hover_handler);

    destroy_click_listener(click_on_listener);
    destroy_click_listener(click_off_listener);

    destroy_key_pressed_listeners(key_listener_pairs);
    destroy_key_pressed_listeners(super_key_listeners);
    destroy_key_pressed_listeners(alt_key_listeners);

    for (auto key_listener_pair: key_listener_pairs) {
        listener_visual_infos.erase(key_listener_pair.key_listener);
    }

    listener_visual_infos.erase(key_escape_listener);
    listener_visual_infos.erase(key_left_alt_listener);
    listener_visual_infos.erase(key_left_super_listener);
    listener_visual_infos.erase(key_backspace_listener);
    listener_visual_infos.erase(key_left_listener);
    listener_visual_infos.erase(key_right_listener);

    destroy_binding(&rect.height);
    destroy_binding(&rect.width);
    destroy_binding(&drawn_text);
    destroy_binding(&text_x_end);
    destroy_binding(&text_y);
    destroy_binding(&text_x);
    end_data_sync();
}

void LookupBox::select() {
    selected = true;
    character_index = 0;
    float new_character_index =
            0.5f + map_range(GetMousePosition().x, text_x, text_x_end, 0, (float) text.size());
    character_index = (int) new_character_index;

    create_key_pressed_listeners(key_listener_pairs);
}

void LookupBox::unselect() {
    selected = false;
    destroy_key_pressed_listeners(key_listener_pairs);
}

void LookupBox::on_text_change() {
    update_listenable(&text, text);
    end_data_sync();

    if (on_lookup) on_lookup(text);
}

void LookupBox::draw() {
    DrawRectangleRec(rect, SECONDARY_COLOR);

    Color text_color = text.empty() ? GRAY : WHITE;
    DrawTextEx(font, drawn_text.c_str(), Vector2{text_x, text_y}, (float) font.baseSize / 2, 0, text_color);

//    DrawLineV(Vector2{rect.x, rect.y + rect.height}, Vector2{rect.x + rect.width, rect.y+ rect.height}, YELLOW);

    if (selected) {
        float selector_line_x = text_x + font_width * (float) character_index;
        DrawLineEx(
                Vector2{selector_line_x, text_y},
                Vector2{selector_line_x, text_y + font_height},
                1,
                WHITE
        );
    }
}