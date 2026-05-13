#pragma once

#ifndef OPTO_USER_INPUT_H
#define OPTO_USER_INPUT_H

#include "stds.h"
#include "globals.h"

typedef struct {
    long long last_click_millis;
    int click_streak;
} MultiClick;

void init_multi_click(MultiClick* m);
int check_clicked_n_times(MultiClick *m, int max_clicks);
//
////extern unordered_set<Event*> click_listeners;
////extern unordered_set<Event*> double_click_listeners;
//
//void tick_user_input();
//
////void create_key_pressed_listener(int key, Event *listener);
//
#endif //OPTO_USER_INPUT_H
