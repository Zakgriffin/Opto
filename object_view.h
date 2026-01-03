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
#include "assign.h"
#include "integer.h"
#include "run.h"
#include "declare.h"
#include "if.h"
#include "loop.h"
#include "while.h"
#include "repeat.h"
#include "greater_than.h"
#include "arm_compile.h"
#include "event.h"

extern unordered_map<void *, ObjectView *> object_to_view;
extern unordered_map<string, void **> name_to_object_handle;

typedef enum {
    NONE,
    DO_THEN,
    ADD,
    ASSIGN,
    RUN,
    INTEGER,
    STRING,
    DECLARE,
    IF,
    LOOP,
    WHILE,
    REPEAT,
    GREATER_THAN,

    CONDITIONAL_JUMP,
    JUMP,
} ObjectType;

typedef struct ObjectView {
    void **object_handle;
    EditableText editable_text;
    Box box;
    Signal box_sig;

    MultiClick multi_click;
    vector<string> potential_lookup;

    vector<Listener> sub_object_constraints;
    vector<ObjectView *> sub_object_views;
    vector<Box*> sub_boxes;

    vector<Listener> internal_constraints;

    void (*previous_destroy_sub_object_views)(ObjectView *object_view);

    void *context; // ZZZZ don't remove! using space with *magic* pseudo inheritance

} ObjectView;

typedef struct ObjectViewBuilder {
    ObjectType object_type;
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

void include_sub_object_view(ObjectView *object_view, ObjectView *sub_object_view);

void generic_destroy_sub_object_views(ObjectView *object_view);

void quick_layout_right(ObjectView *p, ObjectView* o, Box *o_box, Signal *o_box_sig, Box *s_editable_text_box, Signal *s_editable_text_box_sig);

void include_sub_box(ObjectView *o, Box* sub_box, Signal *sub_box_sig);

Signal *lift_reference(void *object);

void drop_reference(void *object);

// template<typename T, typename Field1Type, typename Field2Type>
// void generic_binode_create_sub_object_views(ObjectView *view, Field1Type T::*x1_ptr, Field2Type T::*x2_ptr) {
//     auto handle = (T **) view->object_handle;
//     auto obj = *handle;
//
//     auto x1_view = new_object_view(&(obj->*x1_ptr));
//     quick_layout_right(view, x1_view, &view->editable_text.box, &view->editable_text.box_sig, &x1_view->editable_text.box, &x1_view->editable_text.box_sig);
//
//     auto x2_view = new_object_view(&(obj->*x2_ptr));
//     quick_layout_right(view, x2_view, &x1_view->box, &x1_view->box_sig, &x2_view->editable_text.box, &x2_view->editable_text.box_sig);
// }

template<typename T, typename... FieldTypes>
void generic_linear_create_views(ObjectView *view, FieldTypes T::*... fields) {
    auto handle = (T**) view->object_handle;
    T* obj = *handle;

    Box* prev_box = &view->editable_text.box;
    auto prev_sig = &view->editable_text.box_sig;

    auto process_field = [&](auto field_ptr) {
        auto current_view = new_object_view((void**) &(obj->*field_ptr));
        quick_layout_right(view, current_view, prev_box, prev_sig, &current_view->editable_text.box, &current_view->editable_text.box_sig);

        prev_box = &current_view->box;
        prev_sig = &current_view->box_sig;
    };

    (process_field(fields), ...);
}

#endif //OPTO_OBJECT_VIEW_H
