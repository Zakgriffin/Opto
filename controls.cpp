#include "controls.h"
#include "globals.h"
#include "user_input.h"

map<Fn *, ListenerVisualInfo> listener_visual_infos;

ControlsView::ControlsView(LookupBox *owned_lookup_box) {
    this->owned_lookup_box = owned_lookup_box;
    hovered_key_visual_info = nullptr;

    visuals.insert(this);

    for (KeyVisualInfo *key_visual_info: key_visual_infos_mac) {
        Rectangle rect = key_visual_info->rect;

        auto *hover_rectangle = new Rectangle;

        create_binding(&hover_rectangle->x, {&owned_lookup_box->rect.x},
                       [=]() { hover_rectangle->x = rect.x + owned_lookup_box->rect.x; });
        create_binding(&hover_rectangle->y, {&owned_lookup_box->rect.y, &owned_lookup_box->rect.width},
                       [=]() {
                           hover_rectangle->y = rect.y + owned_lookup_box->rect.y + owned_lookup_box->rect.width;
                       });
        hover_rectangle->width = rect.width;
        hover_rectangle->height = rect.height;

        auto hover_handler = new HoverHandler{
                .is_within=within_rectangle(hover_rectangle),
                .hover_enter=[&, key_visual_info]() {
                    hovered_key_visual_info = key_visual_info;
                },
                .hover_exit=[&, key_visual_info]() {
                    // TODO probably better stack/priority cleanup here
                    if (hovered_key_visual_info == key_visual_info) {
                        hovered_key_visual_info = nullptr;
                    }
                }
        };

        create_hover_handler(hover_handler);
    }
}

ControlsView::~ControlsView() {
    // TODO better cleanup

    visuals.erase(visuals.find(this));
}

void ControlsView::draw() {
    float padding = 2;
    for (KeyVisualInfo *key_visual_info: key_visual_infos_mac) {
        Rectangle key_rect = key_visual_info->rect;
        Rectangle padded_rect = {
                .x=owned_lookup_box->rect.x + key_rect.x + padding,
                .y=owned_lookup_box->rect.y + owned_lookup_box->rect.height + key_rect.y + padding,
                .width=key_rect.width - padding * 2,
                .height=key_rect.height - padding * 2
        };
        DrawRectangleRec(padded_rect, SECONDARY_COLOR);

        for (auto key_pressed_listener: all_key_pressed_listeners[key_visual_info->k]) {
            if (!listener_visual_infos.contains(key_pressed_listener)) continue;

            ListenerVisualInfo listener_visual_info = listener_visual_infos[key_pressed_listener];

            DrawRectangleRec(padded_rect, listener_visual_info.color);
        }

        DrawTextEx(
                font,
                key_visual_info->key_name.c_str(),
                Vector2{
                        .x=owned_lookup_box->rect.x +
                           center(key_rect.x, key_rect.width, font_width * (float) key_visual_info->key_name.size()),
                        .y=owned_lookup_box->rect.y + center(key_rect.y, key_rect.height, font_height) +
                           (font_height * 1.25f),
                },
                (float) font.baseSize / 2,
                0, WHITE
        );

        if (hovered_key_visual_info) {
            for (auto key_pressed_listener: all_key_pressed_listeners[hovered_key_visual_info->k]) {
                Vector2 mouse = GetMousePosition();
                float offset = 10;
                Vector2 eh = {mouse.x + offset, mouse.y + offset};
                ListenerVisualInfo x = listener_visual_infos[key_pressed_listener];

                Rectangle rect_hover = {
                        .x = eh.x,
                        .y = eh.y,
                        .width = font_width * (float) x.description.size(),
                        .height = font_height,
                };

                DrawRectangleRec(rect_hover, SECONDARY_COLOR);

                DrawTextEx(font, x.description.c_str(), {eh.x, eh.y}, font_height, 0, WHITE);
            }
        }
    }
}

float key_size = 40;
float tab_width = key_size * 1.5f;

float caps_width = key_size * (7.f / 4.f);
float shift_width = caps_width + key_size / 2;
float command_width = shift_width - key_size;
float space_width = key_size * 5;

auto v(float row, float offset, float height) {
    return [=](int k, string key_name, float index) {
        return new KeyVisualInfo{
                .k=k,
                .key_name = std::move(key_name),
                .rect={.x=key_size * index + offset, .y=row * key_size, .width=key_size, .height=height}
        };
    };
}

auto v0 = v(0, 0, key_size / 2);
auto v1 = v(0.5, 0, key_size);
auto v2 = v(1.5, tab_width, key_size);
auto v3 = v(2.5, caps_width, key_size);
auto v4 = v(3.5, shift_width, key_size);

auto v5(int k, string key_name, float x, float width) {
    return new KeyVisualInfo{
            .k=k,
            .key_name = std::move(key_name),
            .rect={.x=x, .y=4.5f * key_size, .width=width, .height=key_size}
    };
}

auto v_arrow(int k, string key_name, float offset_x, float offset_y) {
    return new KeyVisualInfo{k, std::move(key_name), {
            .x=offset_x + key_size * 4 + command_width * 2 + space_width,
            .y=offset_y + 4.5f * key_size,
            .width=key_size,
            .height=key_size / 2
    }};
}

vector<KeyVisualInfo *> key_visual_infos_mac{
        v0(KEY_ESCAPE, "esc", 0),

        v1('`', "`", 0),
        v1('1', "1", 1),
        v1('2', "2", 2),
        v1('3', "3", 3),
        v1('4', "4", 4),
        v1('5', "5", 5),
        v1('6', "6", 6),
        v1('7', "7", 7),
        v1('8', "8", 8),
        v1('9', "9", 9),
        v1('0', "0", 10),
        v1('-', "-", 11),
        v1('=', "=", 12),
        new KeyVisualInfo{.k=KEY_BACKSPACE, .key_name = "delete", .rect= {.x=key_size * 13, .y=key_size *
                                                                                               0.5f, .width=tab_width, .height=key_size}},

        new KeyVisualInfo{.k=KEY_TAB, .key_name = "tab", .rect= {.x=0, .y=key_size *
                                                                          1.5f, .width=tab_width, .height=key_size}},
        v2('Q', "q", 0),
        v2('W', "w", 1),
        v2('E', "e", 2),
        v2('R', "r", 3),
        v2('T', "t", 4),
        v2('Y', "y", 5),
        v2('U', "u", 6),
        v2('I', "i", 7),
        v2('O', "o", 8),
        v2('P', "p", 9),
        v2('[', "[", 10),
        v2(']', "]", 11),
        v2('\\', "\\", 12),

        new KeyVisualInfo{.k=KEY_CAPS_LOCK, .key_name = "caps", .rect= {.x=0, .y=key_size *
                                                                                 2.5f, .width=caps_width, .height=key_size}},
        v3('A', "a", 0),
        v3('S', "s", 1),
        v3('D', "d", 2),
        v3('F', "f", 3),
        v3('G', "g", 4),
        v3('H', "h", 5),
        v3('J', "j", 6),
        v3('K', "k", 7),
        v3('L', "l", 8),
        v3(';', ";", 9),
        v3('\'', "'", 10),
        new KeyVisualInfo{.k=KEY_ENTER, .key_name = "enter", .rect= {.x=caps_width + key_size * 11, .y=key_size *
                                                                                                       2.5f, .width=caps_width, .height=key_size}},

        new KeyVisualInfo{.k=KEY_LEFT_SHIFT, .key_name = "shift", .rect= {.x=0, .y=key_size *
                                                                                   3.5f, .width=shift_width, .height=key_size}},
        v4('Z', "z", 0),
        v4('X', "x", 1),
        v4('C', "c", 2),
        v4('V', "v", 3),
        v4('B', "b", 4),
        v4('N', "n", 5),
        v4('M', "m", 6),
        v4(',', ",", 7),
        v4('.', ".", 8),
        v4('/', "/", 9),
        new KeyVisualInfo{.k=KEY_RIGHT_SHIFT, .key_name = "shift", .rect= {.x=shift_width + key_size * 10, .y=key_size *
                                                                                                              3.5f, .width=shift_width, .height=key_size}},
        v5(KEY_NULL, "fn", 0, key_size), // not real apparently
        v5(KEY_LEFT_CONTROL, "ctrl", key_size, key_size),
        v5(KEY_LEFT_ALT, "opt", key_size * 2, key_size),
        v5(KEY_LEFT_SUPER, "cmd", key_size * 3, command_width),
        v5(' ', " ", key_size * 3 + command_width, space_width),
        v5(KEY_RIGHT_SUPER, "cmd", key_size * 3 + command_width + space_width, command_width),
        v5(KEY_RIGHT_ALT, "opt", key_size * 3 + command_width * 2 + space_width, key_size),

        v_arrow(KEY_LEFT, "", 0, key_size * 0.5f),
        v_arrow(KEY_UP, "", key_size, 0),
        v_arrow(KEY_DOWN, "", key_size, key_size * 0.5f),
        v_arrow(KEY_RIGHT, "", key_size * 2, key_size * 0.5f),
};