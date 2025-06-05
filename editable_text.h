#pragma once

#ifndef OPTO_EDITABLE_TEXT_H
#define OPTO_EDITABLE_TEXT_H

#include "stds.h"
#include "globals.h"
#include "event.h"
#include "user_input.h"

typedef struct {
    string text;
    int character_index;
    Box box;
    Color color;

    Signal text_sig;
    Signal box_sig;
//    Signal position_sig;
//    Signal size_sig;
    vector<Listener> internal_listeners;

    Signal key_pressed_sig;
} EditableText;

extern EditableText *selected_editable_text;

void initialize_editable_text(EditableText *e);
void finalize_editable_text(EditableText *e);

#endif //OPTO_EDITABLE_TEXT_H
