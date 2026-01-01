#include "greater_than.h"

ObjectViewBuilder greater_than_object_view_builder = ObjectViewBuilder{GREATER_THAN, "gt", greater_than_create_simple, greater_than_create_sub_object_views, greater_than_destroy_sub_object_views};

void *greater_than_create_simple() {
    auto greater_than = new GreaterThan{.left = nullptr, .right = nullptr};
    object_to_type.insert({greater_than, GREATER_THAN});
    return greater_than;
}

void greater_than_create_sub_object_views(ObjectView *greater_than_view) {
    auto greater_than_handle = (GreaterThan **) greater_than_view->object_handle;
    auto greater_than = *greater_than_handle;

    auto left_view = new_object_view((void **) &greater_than->left);
    quick_layout_right(greater_than_view, left_view, &greater_than_view->editable_text.box, &greater_than_view->editable_text.box_sig, &left_view->editable_text.box, &left_view->editable_text.box_sig);

    auto right_view = new_object_view((void **) &greater_than->right);
    quick_layout_right(greater_than_view, right_view, &left_view->box, &left_view->box_sig, &right_view->editable_text.box, &right_view->editable_text.box_sig);
}

void greater_than_destroy_sub_object_views(ObjectView *greater_than_view) {
    generic_destroy_sub_object_views(greater_than_view);
}