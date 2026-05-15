#pragma once

#include "globals.h"


typedef struct ObjectViewTemp {
    void* object = nullptr;
    Box box;
    
    ObjectViewTemp(void* object) {
        this->object = object;
    }
    virtual ~ObjectViewTemp() = default;
    
    virtual void accept_input(InputContext* ctx) {}
    virtual void layout() {}
    virtual void draw() {}
    virtual ObjectViewTemp* wants_replace() { return nullptr; }
} ObjectViewTemp; 

extern ObjectViewTemp *selected_object_view_temp;

struct EditableTextTemp;

void draw_view_box(ObjectViewTemp* v);
void accept_click_select(InputContext* ctx, ObjectViewTemp* v);
void layout_single_text_box(ObjectViewTemp* parent, EditableTextTemp* text_box, int nudge);