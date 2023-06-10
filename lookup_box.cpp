#include "lookup_box.h"

#include <raylib.h>
#include <string>
#include <utility>
#include <vector>
#include "globals.h"
#include "do_then.h"
#include "constraint.h"
#include "controls.h"

using namespace std;

LookupBox::LookupBox(string prompt) {
    // state initialization
    text = "";
    character_index = 0;
    owning_object_view = nullptr;
    this->prompt = std::move(prompt);
    selected = false;

    // constants and constraints
    pad_x = 5;
    pad_y = 3;

    make_listenable(&rect.x);
    make_listenable(&rect.y);
    make_listenable(&rect.width);
    make_listenable(&rect.height);

    make_listenable(&text_x);
    make_listenable(&text_y);
    create_binding(&text_x, {&rect.x}, [&]() { text_x = rect.x + pad_x; });
    create_binding(&text_y, {&rect.y}, [&]() { text_y = rect.y + pad_y; });

    make_listenable(&text_x_end);
    create_binding(&text_x_end, {&rect.x, &rect.width}, [&]() { text_x_end = rect.x + rect.width; });

    // event handlers
    key_escape_handler = new F([&]() {
        unselect();
    });
    handler_visual_infos[key_escape_handler] = {BLUE, "unselect lookup box"};

    key_right_handler = new F([&]() {
        if (character_index < text.size()) character_index++;
    });
    handler_visual_infos[key_right_handler] = {BLUE, "move to next character"};

    key_left_handler = new F([&]() {
        if (character_index > 0) character_index--;
    });
    handler_visual_infos[key_left_handler] = {BLUE, "move to previous character"};

    key_backspace_handler = new F([&]() {
        if (character_index > 0) {
            text.erase(character_index - 1, 1);
            character_index--;
            on_text_change();
        }
    });
    handler_visual_infos[key_backspace_handler] = {BLUE, "delete character"};

    // TODO stack/priority based modifier keys
    key_left_super_handler = new F([&]() {
        remove_key_pressed_handlers(key_handler_pairs);
        add_key_pressed_handlers(super_key_handlers);
        add_key_released_handler(KEY_LEFT_SUPER, release_super);
    });
    handler_visual_infos[key_left_super_handler] = {GetColor(0xA020F0FF), "super hotkeys"};

    key_left_alt_handler = new F([&]() {
        remove_key_pressed_handlers(key_handler_pairs);
        add_key_pressed_handlers(alt_key_handlers);
        add_key_released_handler(KEY_LEFT_ALT, release_alt);
    });
    handler_visual_infos[key_left_alt_handler] = {GetColor(0xA020F0FF), "alt hotkeys"};

    key_handler_pairs = {
            {KEY_ESCAPE,     key_escape_handler},
            {KEY_RIGHT,      key_right_handler},
            {KEY_LEFT,       key_left_handler},
            {KEY_BACKSPACE,  key_backspace_handler},
            {KEY_LEFT_SUPER, key_left_super_handler},
            {KEY_LEFT_ALT,   key_left_alt_handler},
    };

    for (int key = 0x20; key <= 0x7E; key++) {
        char lower = (char) tolower(key);
        auto key_handler = new F([&, lower]() {
            text.insert(character_index, 1, (char) lower);
            character_index++;

            on_text_change();
        });
        key_handler_pairs.push_back({key, key_handler});


        string description = "type character '";
        description += lower;
        description += "'";

        handler_visual_infos[key_handler] = {GetColor(0xDD7700FF), description};;
    }

    // super handlers
    super_key_a_handler = new F([&]() {

    });

    super_key_handlers = {
            {'A', super_key_a_handler},
    };

    // alt handlers

    alt_key_left_handler = new F([&]() {
        character_index = 0;
    });

    alt_key_right_handler = new F([&]() {
        character_index = (int) text.size();
    });

    alt_key_handlers = {
            {KEY_LEFT,  alt_key_left_handler},
            {KEY_RIGHT, alt_key_right_handler},
    };

    // release handlers

    release_super = new F([&]() {
        remove_key_pressed_handlers(super_key_handlers);
        add_key_pressed_handlers(key_handler_pairs);
        remove_key_released_handler(KEY_LEFT_SUPER, release_super);
    });

    release_alt = new F([&]() {
        remove_key_pressed_handlers(alt_key_handlers);
        add_key_pressed_handlers(key_handler_pairs);
        remove_key_released_handler(KEY_LEFT_ALT, release_alt);
    });

    // click handlers

    click_on_listener = new F([&]() {
        select();
    });

    click_off_listener = new F([&]() {
        unselect();
    });

    // hover handlers

    hover_handler = new HoverInfo{
            .is_within=within_rectangle(&rect),
            .hover_enter=[&]() {
                add_click_listener(click_on_listener);
                remove_click_listener(click_off_listener);
                SetMouseCursor(MOUSE_CURSOR_IBEAM);
            },
            .hover_exit=[&]() {
                remove_click_listener(click_on_listener);
                add_click_listener(click_off_listener);
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }
    };

    add_hover_listener(hover_handler);
}

LookupBox::~LookupBox() {
    remove_hover_listener(hover_handler);

    remove_click_listener(click_on_listener);
    remove_click_listener(click_off_listener);

    for (auto key_handler_pair: key_handler_pairs) {
        handler_visual_infos.erase(key_handler_pair.key_handler);
    }
    handler_visual_infos.erase(key_escape_handler);
    handler_visual_infos.erase(key_left_alt_handler);
    handler_visual_infos.erase(key_left_super_handler);
    handler_visual_infos.erase(key_backspace_handler);
    handler_visual_infos.erase(key_left_handler);
    handler_visual_infos.erase(key_right_handler);

    destroy_binding(&text_x_end);
    unmake_listenable(&text_x_end);

    destroy_binding(&text_y);
    destroy_binding(&text_x);
    make_listenable(&text_y);
    make_listenable(&text_x);

    unmake_listenable(&rect.height);
    unmake_listenable(&rect.width);
    unmake_listenable(&rect.y);
    unmake_listenable(&rect.x);
}

void LookupBox::select() {
    selected = true;
    character_index = 0;
    float new_character_index =
            0.5f + map_range(GetMousePosition().x, text_x, text_x_end, 0, (float) text.size());
    character_index = (int) new_character_index;

    add_key_pressed_handlers(key_handler_pairs);
}

void LookupBox::unselect() {
    selected = false;
    printf("gump %d", (int) key_handler_pairs.size());
    remove_key_pressed_handlers(key_handler_pairs);
}

void LookupBox::on_text_change() { // TODO ideally, use string template for constraint
    if (owning_object_view != nullptr) {
        owning_object_view->destroy_view();
        delete owning_object_view;
        owning_object_view = nullptr;
    }

    if (text == "d") {
        owning_object_view = new DoThenView(this);
    } else if (text == "c") {
        owning_object_view = new ControlsView(this);
    }
}

void LookupBox::draw() {
    string drawn_text = text.empty() ? prompt : text;

    float new_width = font_width * (float) drawn_text.size() + pad_x * 2;
    float new_height = (float) font_height + pad_y * 2;
    if (rect.width != new_width || rect.height != new_height) {
        update_listenable(&rect.width, new_width);
        update_listenable(&rect.height, new_height);
        update_all_tracked();
    }

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