#include "declare.h"

ObjectViewBuilder declare_object_view_builder = ObjectViewBuilder{DECLARE, "var", declare_create_simple, declare_create_sub_object_views, declare_destroy_sub_object_views};

void *declare_create_simple() {
    auto declare = new Declare{.name = nullptr};
    object_to_type.insert({declare, DECLARE});
    return declare;
}

void declare_create_sub_object_views(ObjectView *declare_view) {
    auto declare_handle = (Declare **) declare_view->object_handle;
    auto declare = *declare_handle;

    auto augend_view = new_object_view((void **) &declare->name);
    quick_layout_right(declare_view, augend_view, &declare_view->editable_text.box, &declare_view->editable_text.box_sig, &augend_view->editable_text.box, &augend_view->editable_text.box_sig);
}

void declare_destroy_sub_object_views(ObjectView *declare_view) {
    generic_destroy_sub_object_views(declare_view);
}