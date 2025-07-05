#include "assign.h"

ObjectViewBuilder assign_object_view_builder = ObjectViewBuilder{ASSIGN, "=", assign_create_simple, assign_create_sub_object_views, assign_destroy_sub_object_views};

void *assign_create_simple() {
    auto assign = new Assign{.grantee = nullptr, .grantor = nullptr};
    object_to_type.insert({assign, ASSIGN});
    return assign;
}

void assign_create_sub_object_views(ObjectView *assign_view) {
    auto assign_handle = (Assign **) assign_view->object_handle;
    auto assign = *assign_handle;

    auto grantee_view = new_object_view((void **) &assign->grantee);
    quick_layout_right(assign_view, grantee_view, &assign_view->editable_text.box, &assign_view->editable_text.box_sig, &grantee_view->editable_text.box, &grantee_view->editable_text.box_sig);

    auto grantor_view = new_object_view((void **) &assign->grantor);
    quick_layout_right(assign_view, grantor_view, &grantee_view->box, &grantee_view->box_sig, &grantor_view->editable_text.box, &grantor_view->editable_text.box_sig);
}

void assign_destroy_sub_object_views(ObjectView *assign_view) {
    generic_destroy_sub_object_views(assign_view);
}