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
    Rectangle box;
    Color color;

    Signal text_sig;
    Signal size_sig;
} EditableText;

extern EditableText* selected_editable_text;

void init_editable_text(EditableText *e);

#endif //OPTO_EDITABLE_TEXT_H
