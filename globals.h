#pragma once

#ifndef OPTO_GLOBALS_H
#define OPTO_GLOBALS_H

#include "stds.h"

typedef enum {
    OBJECT_VIEW,
    EDITABLE_TEXT,
} EditMode;

typedef struct {
    float x_min;
    float x_max;
    float y_min;
    float y_max;
} Box;

Rectangle box_to_rectangle(Box box);

extern unordered_map<void *, string> tracked_objects;
#define track(obj) { \
    string log_entry = string(__FILE__) + ":" + to_string(__LINE__); \
    tracked_objects.insert({obj, log_entry}); \
}

extern int ZZZZ_debug_color;

extern Color BACKGROUND_COLOR;
extern Color BOX_COLOR;

extern Font font;

extern int key_pressed;
extern int mouse_cursor;
extern bool mouse_clicked_during_input;
extern EditMode edit_mode;

bool is_within_box(Vector2 point, Box box);

void box_layout_right(Box *super, Box *sub);
void box_layout_under(Box *super, Box *sub);

Box enclosing_box(Box b1, Box b2);

void move_box_x(Box* box, float x_min);

void move_box_y(Box* box, float y_min);

template<typename T>
T map_range(T n, T start1, T stop1, T start2, T stop2) {
    return ((n - start1) / (stop1 - start1)) * (stop2 - start2) + start2;
}

#endif //OPTO_GLOBALS_H
