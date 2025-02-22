#include "user_input.h"

unordered_set<Event*> click_listeners;
unordered_set<Event*> single_click_listeners;
unordered_set<Event*> double_click_listeners;

unordered_set<Event*> destroy_listeners;

map<int, unordered_set<Event *>> all_key_pressed_listeners;
map<int, unordered_set<Event *>> all_key_released_listeners;

int last_frame_clicked = 0;

void register_click_listener() {
//    events.insert_next(new Event([]() {
//        if (IsMouseButtonPressed(0)) {
//        events.insert_all_next(click_listeners);
//    });

//    if (IsMouseButtonPressed(0)) {
//        if (last_frame_clicked < 20) {
//            for (const auto &double_click_listener: double_click_listeners) double_click_listener();
//        } else {
//            for (const auto &click_listener: click_listeners) click_listener();
//        }
//        last_frame_clicked = 0;
//    }
//    last_frame_clicked++;
//
//    for (const auto &key_pressed_listeners_entry: all_key_pressed_listeners) {
//        auto key_pressed = key_pressed_listeners_entry.first;
//        auto key_pressed_listeners = key_pressed_listeners_entry.second;
//        if (IsKeyPressed(key_pressed)) {
//            for (auto key_pressed_listener: key_pressed_listeners) (*key_pressed_listener)();
//        }
//    }
}

void create_key_pressed_listener(int key, Event *listener) {
    all_key_pressed_listeners[key].insert(listener);
}