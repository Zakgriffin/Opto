#pragma once

#include "event.h"
#include "globals.h"

typedef struct EditableText {
    string text;
    int character_index;
    Box box;
    ray::Color color;

    Signal text_input_sig;
    Signal text_sig;
    Signal box_sig;

    vector<Listener> internal_listeners;

    Signal key_pressed_sig;

    ray::Color underline_color;
    vector<function<bool()>> on_keys;
} EditableText;

extern EditableText *selected_editable_text;

void initialize_editable_text(EditableText *e);
void finalize_editable_text(EditableText *e);
