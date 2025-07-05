#include "add.h"

ObjectViewBuilder add_object_view_builder = ObjectViewBuilder{ADD, "add", add_create_simple, add_create_sub_object_views, add_destroy_sub_object_views};

void *add_create_simple() {
    auto add = new Add{.augend = nullptr, .addend = nullptr};
    object_to_type.insert({add, ADD});
    return add;
}

void add_create_sub_object_views(ObjectView *add_view) {
    auto add_handle = (Add **) add_view->object_handle;
    auto add = *add_handle;

    auto augend_view = new_object_view((void **) &add->augend);
    quick_layout_right(add_view, augend_view, &add_view->editable_text.box, &add_view->editable_text.box_sig, &augend_view->editable_text.box, &augend_view->editable_text.box_sig);

    auto addend_view = new_object_view((void **) &add->addend);
    quick_layout_right(add_view, addend_view, &augend_view->box, &augend_view->box_sig, &addend_view->editable_text.box, &addend_view->editable_text.box_sig);
}

void add_destroy_sub_object_views(ObjectView *add_view) {
    generic_destroy_sub_object_views(add_view);
}