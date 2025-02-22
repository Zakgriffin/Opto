#ifndef OPTO_USER_INPUT_H
#define OPTO_USER_INPUT_H

#include "stds.h"
#include "globals.h"
#include "event.h"

extern unordered_set<Event*> click_listeners;
extern unordered_set<Event*> double_click_listeners;

void tick_user_input();

void create_key_pressed_listener(int key, Event *listener);

#endif //OPTO_USER_INPUT_H
