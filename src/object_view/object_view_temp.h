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