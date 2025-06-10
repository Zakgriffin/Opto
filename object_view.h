#pragma once

#ifndef OPTO_OBJECT_VIEW_H
#define OPTO_OBJECT_VIEW_H

#include "stds.h"
#include "editable_text.h"
#include "unknown.h"
#include "none.h"
#include "do_then.h"
#include "string_.h"
#include "add.h"
#include "integer.h"
#include "event.h"

typedef enum {
    NONE,
    DO_THEN,
    ADD,
} ObjectType;

typedef struct ObjectView {
    void **object_handle;
    EditableText editable_text;
    Box box;
    Signal box_sig;

    MultiClick multi_click;
    vector<string> potential_lookup;
//    ObjectType object_type;

    vector<Listener> sub_object_constraints;
    vector<ObjectView *> sub_object_views;

    vector<Listener> internal_constraints;

    void (*previous_destroy_sub_object_views)(ObjectView *object_view);

    void *context;

} ObjectView;

typedef struct ObjectViewBuilder {
    ObjectType object_type;

//    Unknown *(*to_unknown)(void *);
//
//    void *(*from_unknown)(Unknown *);

    string s;

    void *(*create_simple)();

    void (*create_sub_object_views)(ObjectView *object_view);

    void (*destroy_sub_object_views)(ObjectView *object_view);

} ObjectViewBuilder;

extern ObjectView *selected_object_view;

template<typename T>
struct Shared {
    T o;
    int count;
};

extern map<void *, ObjectType> object_to_type;
extern map<void *, Shared<Signal> *> object_to_signal;

template<typename T>
T *typed(ObjectType type, T *object) {
    object_to_type.insert({object, type});
    return object;
}

ObjectView *new_object_view(void **object_handle);

void destroy_object_view(ObjectView *object_view);

void redo_sub_objects(ObjectView *o, const ObjectViewBuilder &object_view_builder);

void init_object_view_builders();

void make_top_level_object(void *object);

void include_sub_object_view(ObjectView *object_view, ObjectView *sub_object_view);

// maybe rather than destroy object view controlling data
// thats now being used for something else,
// transfer sub objects to new object view
void generic_destroy_sub_object_views(ObjectView *object_view);

#endif //OPTO_OBJECT_VIEW_H
