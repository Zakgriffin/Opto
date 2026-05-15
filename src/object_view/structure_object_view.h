#pragma once

#include "editable_text_temp.h"
#include "globals.h"
#include "object_view_temp.h"


struct StructureObjectView : ObjectViewTemp {
    EditableTextTemp structure_text_box;
    vector<ObjectViewTemp*> field_views;

    StructureObjectView(void* object);
    ~StructureObjectView() = default;

    void accept_input(InputContext* ctx) override;
    void layout() override;
    void draw() override;
    ObjectViewTemp* wants_replace() override;
};

struct AddStructureObjectView : StructureObjectView {
    AddStructureObjectView(void* add);

    void layout() override;
    ObjectViewTemp* wants_replace() override;
};