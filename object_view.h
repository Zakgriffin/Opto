#ifndef OPTO_OBJECT_VIEW_H
#define OPTO_OBJECT_VIEW_H

#include "stds.h"
#include "globals.h"
#include "user_input.h"
#include "object.h"
#include "editable_text.h"

struct ObjectView {
public:
    void *object;
    EditableText editable_text;
    Rectangle box;

    explicit ObjectView(void *object);
};

extern vector<ObjectView *> visuals;

void make_top_level_object(void *object);

void box_touch_left_right(Rectangle *left_box, Rectangle *right_box);

void box_touch_up_down(Rectangle *up_box, Rectangle *down_box);

void box_align_left(Rectangle *box_1, Rectangle *box_2);

void box_align_up(Rectangle *box_1, Rectangle *box_2);

#endif //OPTO_OBJECT_VIEW_H
