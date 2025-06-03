#include "user_input.h"

bool is_within_rect(Vector2 point, Rectangle *rect) {
    return point.x > rect->x && point.x < rect->x + rect->width && point.y > rect->y && point.y < rect->y + rect->height;
}

void init_multi_click(MultiClick *m) {
    m->last_click_millis = 0;
    m->click_streak = 0;
}

long long millis() {
    auto now = high_resolution_clock::now();
    return duration_cast<milliseconds>(now.time_since_epoch()).count();
}

int BETWEEN_MULTI_CLICK_TIME = 250;
int check_clicked_n_times(MultiClick *m, int max_clicks) {
    long long current_millis = millis();

    auto dif = current_millis - m->last_click_millis;
    if (IsMouseButtonPressed(0)) {
        if (m->click_streak == 0 || dif < BETWEEN_MULTI_CLICK_TIME) m->click_streak++;
        m->last_click_millis = current_millis;
    } else {
        if ((m->click_streak > 0 && dif > BETWEEN_MULTI_CLICK_TIME) || m->click_streak == max_clicks) {
            auto streak = m->click_streak;
            m->click_streak = 0;
            return streak;
        }
    }

    return 0;

//    {
//        if (m->click_streak > 0) {
//            cout << m->click_streak;
//            m->click_streak = 0;
//            return false;
//        }
//    }


    return true;
}

//
////unordered_set<Event *> click_listeners;
////unordered_set<Event *> single_click_listeners;
////unordered_set<Event *> double_click_listeners;
////
////map<int, unordered_set<Event *>> all_key_pressed_listeners;
////map<int, unordered_set<Event *>> all_key_released_listeners;
//
//int last_frame_clicked = 0;
//
//void register_click_listener() {
////    if (IsMouseButtonPressed(0)) {
////        if (last_frame_clicked < 20) {
////            for (const auto &double_click_listener: double_click_listeners) (*double_click_listener)();
////        } else {
////            for (const auto &click_listener: click_listeners) (*click_listener)();
////        }
////        last_frame_clicked = 0;
////    }
////    last_frame_clicked++;
////
////    for (const auto &key_pressed_listeners_entry: all_key_pressed_listeners) {
////        auto key_pressed = key_pressed_listeners_entry.first;
////        auto key_pressed_listeners = key_pressed_listeners_entry.second;
////        if (IsKeyPressed(key_pressed)) {
////            for (auto key_pressed_listener: key_pressed_listeners) (*key_pressed_listener)();
////        }
////    }
//}
//
////void create_key_pressed_listener(int key, Event *listener) {
////    all_key_pressed_listeners[key].insert(listener);
////}