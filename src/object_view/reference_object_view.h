#pragma once

#include "editable_text_temp.h"
#include "globals.h"
#include "object_view_temp.h"

struct ReferenceObjectView : ObjectViewTemp {
    EditableTextTemp reference_text_box;

    ReferenceObjectView(void* obj);

    void accept_input(InputContext* ctx) override;
    void layout() override;
    void draw() override;
};
