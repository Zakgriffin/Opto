#ifndef OPTO_LOOKUP_BOX_H
#define OPTO_LOOKUP_BOX_H

#include <vector>
#include <string>
#include "raylib.h"
#include "object_view.h"
#include "visual.h"
#include "globals.h"
#include "controls.h"
#include "constraint.h"

using namespace std;

struct LookupBox : public Visual {
public:
    // logic
    string text;
    bool selected;
    int character_index;
    string prompt;

    F *click_on_listener;
    F *click_off_listener;

    vector<KeyHandlerPair*> key_handler_pairs;
    vector<KeyHandlerPair*> super_key_handlers, alt_key_handlers;
    F *key_escape_handler, *key_right_handler, *key_left_handler, *key_backspace_handler, *key_left_super_handler, *key_left_alt_handler;
    F *super_key_a_handler;
    F *alt_key_left_handler, *alt_key_right_handler;
    F *release_super, *release_alt;
    HoverInfo *hover_handler;

    float pad_x, pad_y, text_x, text_y, text_x_end;
    string drawn_text;

    function<void(string)> on_lookup;
    CleanupContext c;

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
