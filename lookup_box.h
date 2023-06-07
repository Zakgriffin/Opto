#ifndef OPTO_LOOKUP_BOX_H
#define OPTO_LOOKUP_BOX_H

#include <vector>
#include <string>
#include "raylib.h"
#include "object_view.h"
#include "visual.h"
#include "globals.h"
#include "controls.h"

using namespace std;

struct LookupBox : public Visual {
public:
    vector<float *> tracked_for_visuals;

    // logic
    string text;
    bool selected;
    int character_index;
    string prompt;

    F *click_on_listener;
    F *click_off_listener;

    vector<KeyHandlerPair> key_handlers;
    vector<KeyHandlerPair> super_key_handlers, alt_key_handlers;
    F *key_escape_handler, *key_right_handler, *key_left_handler, *key_backspace_handler, *key_left_super_handler, *key_left_alt_handler;
    F *super_key_a_handler;
    F *alt_key_left_handler, *alt_key_right_handler;
    F *release_super, *release_alt;

    float pad_x, pad_y, text_x{}, text_y{}, text_x_end{};

    // visual
    Rectangle rect{};

    ObjectView *owning_object_view;

    void on_text_change();

    void select();

    void unselect();

    void draw() override;

    explicit LookupBox(string prompt);

    ~LookupBox();
};

#endif //OPTO_LOOKUP_BOX_H
