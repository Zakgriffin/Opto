#include "add.h"



void add_create_sub_object_views(ObjectView *add_view) {
    auto add_handle = (Add **) add_view->object_handle;
    auto add = new Add{.augend = nullptr, .addend = nullptr};
    *add_handle = add;

    auto augend_view = new_object_view((void **) &add->augend);
    add_view->sub_object_constraints.push_back(create_listener({&add_view->editable_text.box_sig}, new function<void(void)>([=]() {
        box_layout_right(&add_view->editable_text.box, &augend_view->editable_text.box);
        signal_update(&augend_view->editable_text.box_sig);
    })));
    include_sub_object_view(add_view, augend_view);

    auto addend_view = new_object_view((void **) &add->addend);
    add_view->sub_object_constraints.push_back(create_listener({&augend_view->box_sig}, new function<void(void)>([=]() {
        box_layout_right(&augend_view->box, &addend_view->editable_text.box);
        signal_update(&addend_view->editable_text.box_sig);
    })));
    include_sub_object_view(add_view, addend_view);
}

void add_destroy_sub_object_views(ObjectView *add_view) {
    auto add_handle = (Add **) add_view->object_handle;
    generic_destroy_sub_object_views(add_view, add_handle);
}