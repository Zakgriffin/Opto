#pragma once

#ifndef OPTO_OBJECT_VIEW_H
#define OPTO_OBJECT_VIEW_H

#include "stds.h"
#include "editable_text.h"
#include "unknown.h"
#include "do_then.h"
#include "string_.h"
#include "add.h"
#include "integer.h"
#include "event.h"

typedef enum {
    UNKNOWN,
    DO_THEN,
    STRING
} ObjectType;

typedef struct ObjectView {
    void *object_handle;
    EditableText editable_text;
    Box box;
    Signal box_sig;

//    Signal position_sig;
//    Signal size_sig;
    MultiClick multi_click;
    vector<string> potential_lookup;
    ObjectType object_type;

    vector<Box *> inner_boxes;
    vector<Listener> sub_object_constraints;
    vector<ObjectView *> sub_object_views;

    vector<Listener> internal_constraints;

    void (*previous_destroy_sub_object_views)(ObjectView *object_view);

    vector<Signal *> bounding_box_signals;

    void *context;
} ObjectView;

typedef struct {
//    ObjectType object_type;

//    Unknown *(*to_unknown)(void *);
//
//    void *(*from_unknown)(Unknown *);

    void (*create_sub_object_views)(ObjectView *object_view);

    void (*destroy_sub_object_views)(ObjectView *object_view);

} UnknownConverter;

extern ObjectView *selected_object_view;

ObjectView *new_object_view(void **object_handle);

void destroy_object_view(ObjectView *object_view);

void init_unknown_converters();

void make_top_level_object(void *object);

void include_sub_object_view(ObjectView *object_view, ObjectView *sub_object_view);

// maybe rather than destroy object view controlling data
// thats now being used for something else,
// transfer sub objects to new object view
template<typename T>
void generic_destroy_sub_object_views(ObjectView *object_view, T **handle) {
    for (const auto &sub_object_constraint: object_view->sub_object_constraints) {
        destroy_listener(sub_object_constraint);
    }
    for (auto sub: object_view->sub_object_views) {
        destroy_object_view(sub);
    }
    delete *handle;

    object_view->sub_object_constraints.clear();
    object_view->sub_object_views.clear();
}

#endif //OPTO_OBJECT_VIEW_H
