#pragma once

#ifndef OPTO_GLOBALS_H
#define OPTO_GLOBALS_H

#include "stds.h"

enum EditMode {
    OBJECT_VIEW,
    EDITABLE_TEXT,
};

extern unordered_map<void *, string> tracked_objects;
#define track(obj) { \
    string log_entry = string(__FILE__) + ":" + to_string(__LINE__); \
    tracked_objects.insert({obj, log_entry}); \
}

extern Color BACKGROUND_COLOR;
extern Color BOX_COLOR;

extern Font font;

extern int key_pressed;
extern int mouse_cursor;
extern bool mouse_clicked_during_input;
extern EditMode edit_mode;

#endif //OPTO_GLOBALS_H
