#include "lookup_box.h"

#include <raylib.h>
#include <string>
#include <utility>
#include <vector>
#include "globals.h"
#include "constraint.h"
#include "controls.h"

using namespace std;

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

    c.make_binding(&text_x, {&rect.x}, [&]() { text_x = rect.x + pad_x; });
    c.make_binding(&text_y, {&rect.y}, [&]() { text_y = rect.y + pad_y; });

    c.make_binding(&text_x_end, {&rect.x, &rect.width}, [&]() { text_x_end = rect.x + rect.width; });

    c.make_binding(&drawn_text, {&text, &prompt},
                   [&]() { drawn_text = text.empty() ? prompt : text; });

    c.make_binding(&rect.width, {&drawn_text},
                   [&]() { rect.width = font_width * (float) drawn_text.size() + pad_x * 2; });
    c.make_binding(&rect.height, {},
                   [&]() { rect.height = (float) font_height + pad_y * 2; });

    // event listeners
    key_escape_listener = new F([&]() {
        unselect();
    });
    listener_visual_infos[key_escape_listener] = {BLUE, "unselect lookup box"};

    key_right_listener = new F([&]() {
        if (character_index < text.size()) character_index++;
    });
    listener_visual_infos[key_right_listener] = {BLUE, "move to next character"};

    key_left_listener = new F([&]() {
        if (character_index > 0) character_index--;
    });
    listener_visual_infos[key_left_listener] = {BLUE, "move to previous character"};

    key_backspace_listener = new F([&]() {
        if (character_index > 0) {
            text.erase(character_index - 1, 1);
            character_index--;
            on_text_change();
        }
    });
    listener_visual_infos[key_backspace_listener] = {BLUE, "delete character"};

    // TODO stack/priority based modifier keys
    key_left_super_listener = new F([&]() {
        destroy_key_pressed_listeners(key_listener_pairs);
        create_key_pressed_listeners(super_key_listeners);
        create_key_released_listener(KEY_LEFT_SUPER, release_super);
    });
    listener_visual_infos[key_left_super_listener] = {GetColor(0xA020F0FF), "super hotkeys"};

    key_left_alt_listener = new F([&]() {
        destroy_key_pressed_listeners(key_listener_pairs);
        create_key_pressed_listeners(alt_key_listeners);
        create_key_released_listener(KEY_LEFT_ALT, release_alt);
    });
    listener_visual_infos[key_left_alt_listener] = {GetColor(0xA020F0FF), "alt hotkeys"};

    key_listener_pairs = {
            {KEY_ESCAPE, key_escape_listener},
            {KEY_RIGHT, key_right_listener},
            {KEY_LEFT, key_left_listener},
            {KEY_BACKSPACE, key_backspace_listener},
            {KEY_LEFT_SUPER, key_left_super_listener},
            {KEY_LEFT_ALT, key_left_alt_listener},
    };

    for (int key = 0x20; key <= 0x7E; key++) {
        char lower = (char) tolower(key);
        auto key_listener = new F([&, lower]() {
            text.insert(character_index, 1, (char) lower);
            character_index++;


            on_text_change();
        });
        key_listener_pairs.push_back({key, key_listener});


        string description = "type character '";
        description += lower;
        description += "'";

        listener_visual_infos[key_listener] = {GetColor(0xDD7700FF), description};;
    }

    // super listeners
    super_key_a_listener = new F([&]() {

    });

    super_key_listeners = {
            {'A', super_key_a_listener},
    };

    // alt listeners

    alt_key_left_listener = new F([&]() {
        character_index = 0;
    });

    alt_key_right_listener = new F([&]() {
        character_index = (int) text.size();
    });

    alt_key_listeners = {
            {KEY_LEFT, alt_key_left_listener},
            {KEY_RIGHT, alt_key_right_listener},
    };

    // release listeners

    release_super = new F([&]() {
        destroy_key_pressed_listeners(super_key_listeners);
        create_key_pressed_listeners(key_listener_pairs);
        destroy_key_released_listener(KEY_LEFT_SUPER, release_super);
    });

    release_alt = new F([&]() {
        destroy_key_pressed_listeners(alt_key_listeners);
        create_key_pressed_listeners(key_listener_pairs);
        destroy_key_released_listener(KEY_LEFT_ALT, release_alt);
    });

    // click listeners

    click_on_listener = new F([&]() {
        select();
    });

    click_off_listener = new F([&]() {
        unselect();
    });

    // hover listeners

    hover_listener = new HoverInfo{
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

    create_hover_listener(hover_listener);

    visuals.insert(this);
}

LookupBox::~LookupBox() {
    visuals.erase(this);

    destroy_hover_listener(hover_listener);

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

    c.destroy_all();
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
    update_all_tracked();

    if (on_lookup) on_lookup(text);
}

void LookupBox::draw() {
    DrawRectangleRec(rect, SECONDARY_COLOR);

    Color text_color = text.empty() ? GRAY : WHITE;
    DrawTextEx(font, drawn_text.c_str(), Vector2{text_x, text_y}, (float) font.baseSize / 2, 0, text_color);

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