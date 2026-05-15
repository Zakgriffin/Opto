#include "reference_object_view.h"

ReferenceObjectView::ReferenceObjectView(void* obj) : ObjectViewTemp(obj) {
    this->reference_text_box = EditableTextTemp();
}

void ReferenceObjectView::accept_input(InputContext* ctx) {}

void ReferenceObjectView::layout() {

}

void ReferenceObjectView::draw() {}
