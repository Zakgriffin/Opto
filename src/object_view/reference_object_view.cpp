#include "reference_object_view.h"
#include "object_view_temp.h"

static constexpr int NUDGE = 5;

ReferenceObjectView::ReferenceObjectView(void* obj) : ObjectViewTemp(obj) {
    this->reference_text_box.color = REFERENCE_VIEW_COLOR;
}

void ReferenceObjectView::accept_input(InputContext* ctx) {
    this->reference_text_box.accept_input(ctx);
    accept_click_select(ctx, this);
}

void ReferenceObjectView::layout() {
    layout_single_text_box(this, &this->reference_text_box, NUDGE);
}

void ReferenceObjectView::draw() {
    draw_view_box(this);
    this->reference_text_box.draw();
}
