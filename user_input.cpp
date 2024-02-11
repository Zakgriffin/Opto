#include "globals.h"
#include "user_input.h"

map<int, unordered_set<Fn *>> all_key_pressed_listeners;
map<int, unordered_set<Fn *>> all_key_released_listeners;
unordered_set<Fn *> mouse_move_listeners;
unordered_set<Fn *> click_listeners;
unordered_set<Fn *> double_click_listeners;
vector<HoverHandler *> hover_handlers;

//void debug_list_controls() {
//    int key_pressed_listeners_size = 0;
//    for (const auto &key_pressed_listeners: all_key_pressed_listeners) {
//        key_pressed_listeners_size += (int) key_pressed_listeners.second.size();
//    }
//    printf("key_pressed_listeners: %d\n", key_pressed_listeners_size);
//
//    int key_released_listeners_size = 0;
//    for (const auto &key_released_listeners: all_key_released_listeners) {
//        key_released_listeners_size += (int) key_released_listeners.second.size();
//    }
//    printf("key_released_listeners: %d\n", key_released_listeners_size);
//
//    printf("mouse_move_listeners: %d\n", (int) mouse_move_listeners.size());
//    printf("click_listeners: %d\n", (int) click_listeners.size());
//    printf("double_click_listeners: %d\n", (int) double_click_listeners.size());
//    printf("hover_handlers: %d\n", (int) hover_handlers.size());
//}

bool equals(Vector2 a, Vector2 b) {
    return a.x == b.x && a.y == b.y;
}

Vector2 mouse_pos;
int last_frame_clicked = 0;

void tick_controls() {
    Vector2 old_mouse_pos = mouse_pos;
    mouse_pos = GetMousePosition();

    if (!equals(mouse_pos, old_mouse_pos)) {
        for (auto mouse_move_listener: mouse_move_listeners) {
            mouse_move_listener->f();
        }

        for (HoverHandler *hover_handler: hover_handlers) {
            if (hover_handler->is_within(mouse_pos)) {
                if (!hover_handler->was_hovered) {
                    hover_handler->hover_enter();
                    hover_handler->was_hovered = true;
                }
            } else {
                if (hover_handler->was_hovered) {
                    hover_handler->hover_exit();
                    hover_handler->was_hovered = false;
                }
            }
        }
    }

    last_frame_clicked++;
    if (IsMouseButtonPressed(0)) {
        if (last_frame_clicked < 20) {
            for (auto double_click_listener: double_click_listeners) double_click_listener->f();
        } else {
            for (auto click_listener: click_listeners) click_listener->f();
        }
        last_frame_clicked = 0;
    }

    for (const auto &key_pressed_listeners_entry: all_key_pressed_listeners) {
        auto key_pressed = key_pressed_listeners_entry.first;
        auto key_pressed_listeners = key_pressed_listeners_entry.second;
        if (IsKeyPressed(key_pressed)) {
            for (auto key_pressed_listener: key_pressed_listeners) key_pressed_listener->f();
        }
    }

    for (const auto &key_released_listeners_entry: all_key_released_listeners) {
        auto key_released = key_released_listeners_entry.first;
        auto key_released_listeners = key_released_listeners_entry.second;
        if (IsKeyReleased(key_released)) {
            for (auto key_released_listener: key_released_listeners) key_released_listener->f();
        }
    }
}

void create_key_pressed_listener(int key, Fn *listener) {
    all_key_pressed_listeners[key].insert(listener);
}

void destroy_key_pressed_listener(int key, Fn *listener) {
    if (all_key_pressed_listeners[key].contains(listener)) {
        all_key_pressed_listeners[key].erase(all_key_pressed_listeners[key].find(listener));
    }
}

void create_key_released_listener(int key, Fn *listener) {
    all_key_released_listeners[key].insert(listener);
}

void destroy_key_released_listener(int key, Fn *listener) {
    if (all_key_released_listeners[key].contains(listener)) {
        all_key_released_listeners[key].erase(all_key_released_listeners[key].find(listener));
    }
}

void create_key_pressed_listeners(const vector<KeyListenerPair> &key_pressed_listener_pairs) {
    for (auto key_pressed_listener_pair: key_pressed_listener_pairs) {
        create_key_pressed_listener(key_pressed_listener_pair.key, key_pressed_listener_pair.key_listener);
    }
}

void destroy_key_pressed_listeners(const vector<KeyListenerPair> &key_pressed_listener_pairs) {
    for (auto key_pressed_listener_pair: key_pressed_listener_pairs) {
        destroy_key_pressed_listener(key_pressed_listener_pair.key, key_pressed_listener_pair.key_listener);
    }
}

//void create_key_released_listeners(const vector<KeyListenerPair> &key_released_listener_pairs) {
//    for (auto key_released_listener_pair: key_released_listener_pairs) {
//        create_key_released_listener(key_released_listener_pair.key, key_released_listener_pair.key_listener);
//    }
//}
//
//void destroy_key_released_listeners(const vector<KeyListenerPair> &key_released_listener_pairs) {
//    for (auto key_released_listener_pair: key_released_listener_pairs) {
//        destroy_key_released_listener(key_released_listener_pair.key, key_released_listener_pair.key_listener);
//    }
//}


void create_hover_handler(HoverHandler *hover_handler) {
    if (hover_handler->is_within(GetMousePosition())) {
        hover_handler->hover_enter();
        hover_handler->was_hovered = true;
    } else {
        hover_handler->hover_exit();
        hover_handler->was_hovered = false;
    }

    hover_handlers.push_back(hover_handler);
}

void destroy_hover_handler(HoverHandler *hover_handler) {
    hover_handlers.erase(remove(hover_handlers.begin(), hover_handlers.end(), hover_handler), hover_handlers.end());
}

void create_click_listener(Fn *listener) {
    click_listeners.insert(listener);
}

void destroy_click_listener(Fn *listener) {
    if (click_listeners.contains(listener)) {
        click_listeners.erase(click_listeners.find(listener));
    }
}

void create_double_click_listener(Fn *listener) {
    double_click_listeners.insert(listener);
}

void destroy_double_click_listener(Fn *listener) {
    if (double_click_listeners.contains(listener)) {
        double_click_listeners.erase(double_click_listeners.find(listener));
    }
}