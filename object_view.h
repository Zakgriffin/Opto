#pragma once

#ifndef OPTO_OBJECT_VIEW_H
#define OPTO_OBJECT_VIEW_H

#include "stds.h"
#include "editable_text.h"
#include "unknown.h"
#include "do_then.h"
#include "string_.h"
#include "event.h"

typedef enum {
    UNKNOWN,
    DO_THEN,
    STRING
} ObjectType;

typedef struct ObjectView {
    void *object_handle;
    EditableText editable_text;
    Rectangle box;

    Signal position_sig;
    Signal size_sig;
    MultiClick multi_click;
    vector<string> potential_lookup;
    ObjectType object_type;

    set<ObjectView *> sub_object_views;
    vector<Listener> sub_object_constraints;
    vector<Listener> internal_constraints;

    void (*previous_destroy_sub_object_views)(ObjectView *object_view);

    void* context;
} ObjectView;

typedef struct {
    ObjectType object_type;

    Unknown *(*to_unknown)(void *);

    void *(*from_unknown)(Unknown *);

    void (*create_sub_object_views)(ObjectView *object_view);

    void (*destroy_sub_object_views)(ObjectView *object_view);

} UnknownConverter;

extern ObjectView *selected_object_view;

ObjectView *new_object_view(void **object_handle, ObjectType object_type);

void destroy_object_view(ObjectView *object_view);

void init_unknown_converters();

void make_top_level_object(void *object);

void box_touch_left_right(Rectangle *left_box, Rectangle *right_box);

void box_touch_up_down(Rectangle *up_box, Rectangle *down_box);

void box_align_left(Rectangle *box_1, Rectangle *box_2);

void box_align_up(Rectangle *box_1, Rectangle *box_2);

#endif //OPTO_OBJECT_VIEW_H
