#pragma once

#include "globals.h"
#include "object_view_temp.h"
#include "structure_object_view.h"

struct ScatterObjectView : ObjectViewTemp {
    ScatterObjectView(void* object);
};

struct EverythingScatterObjectView : ScatterObjectView {
    vector<ObjectViewTemp*> items;

    EverythingScatterObjectView(void* object);

    void accept_input(InputContext* ctx) override;
    void layout() override;
    void draw() override;
};
