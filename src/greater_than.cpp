#include "greater_than.h"

ObjectViewBuilder greater_than_object_view_builder = ObjectViewBuilder{GREATER_THAN, "gt", greater_than_create_simple, greater_than_create_sub_object_views, greater_than_destroy_sub_object_views};

void *greater_than_create_simple() {
    auto greater_than = new GreaterThan{.left = nullptr, .right = nullptr};
    object_to_type.insert({greater_than, GREATER_THAN});
    return greater_than;
}

void greater_than_create_sub_object_views(ObjectView *greater_than_view) {
    generic_linear_create_views(greater_than_view, &GreaterThan::left, &GreaterThan::right);
}

void greater_than_destroy_sub_object_views(ObjectView *greater_than_view) {
    generic_destroy_sub_object_views(greater_than_view);
}