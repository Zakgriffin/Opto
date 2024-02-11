#ifndef OPTO_USER_INPUT_H
#define OPTO_USER_INPUT_H

#include "globals.h"

extern map<int, unordered_set<Fn *>> all_key_pressed_listeners;

void tick_controls();

//void debug_list_controls();

// listeners

void create_hover_handler(HoverHandler *hover_handler);

void destroy_hover_handler(HoverHandler *hover_handler);

void create_click_listener(Fn *listener);

void destroy_click_listener(Fn *listener);

void create_double_click_listener(Fn *listener);

void destroy_double_click_listener(Fn *listener);

void create_key_pressed_listener(int key, Fn *listener);

void destroy_key_pressed_listener(int key, Fn *listener);

void create_key_released_listener(int key, Fn *listener);

void destroy_key_released_listener(int key, Fn *listener);

void create_key_pressed_listeners(const vector<KeyListenerPair> &key_listener_pairs);

void destroy_key_pressed_listeners(const vector<KeyListenerPair> &key_listener_pairs);

//void create_key_released_listeners(const vector<KeyListenerPair> &key_listener_pairs);

//void destroy_key_released_listeners(const vector<KeyListenerPair> &key_listener_pairs);

#endif //OPTO_USER_INPUT_H
