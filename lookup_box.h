#ifndef OPTO_LOOKUP_BOX_H
#define OPTO_LOOKUP_BOX_H

#include <vector>
#include <string>
#include "raylib.h"
#include "visual.h"
#include "globals.h"
#include "controls.h"
#include "reactivity.h"

using namespace std;

struct LookupBox : public Visual {
public:
    // logic
    string text;
    bool selected;
    int character_index;
    string prompt;

    Fn *click_on_listener;
    Fn *click_off_listener;

    vector<KeyListenerPair> key_listener_pairs;
    vector<KeyListenerPair> super_key_listeners, alt_key_listeners;
    Fn *key_escape_listener, *key_right_listener, *key_left_listener, *key_backspace_listener, *key_left_super_listener, *key_left_alt_listener;
    Fn *super_key_a_listener;
    Fn *alt_key_left_listener, *alt_key_right_listener;
    Fn *release_super, *release_alt;
    HoverHandler *hover_handler;

    float pad_x, pad_y, text_x, text_y, text_x_end;
    string drawn_text;

    function<void(string)> on_lookup;

    // visual
    Rectangle rect;

    void on_text_change();

    void select();

    void unselect();

    void draw() override;

    explicit LookupBox();

    ~LookupBox();
};

#endif //OPTO_LOOKUP_BOX_H
