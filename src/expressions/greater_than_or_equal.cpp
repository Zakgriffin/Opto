#include "object_view.h"
#include "greater_than_or_equal.h"

ObjectViewBuilder greater_than_or_equal_object_view_builder = ObjectViewBuilder{GREATER_THAN_OR_EQUAL, "gte", greater_than_or_equal_create_simple, greater_than_or_equal_create_sub_object_views, greater_than_or_equal_destroy_sub_object_views};

void *greater_than_or_equal_create_simple() {
    auto greater_than_or_equal = new GreaterThanOrEqual{.left = nullptr, .right = nullptr};
    object_to_type.insert({greater_than_or_equal, GREATER_THAN_OR_EQUAL});
    return greater_than_or_equal;
}

void greater_than_or_equal_create_sub_object_views(ObjectView *greater_than_or_equal_view) {
    generic_linear_create_views(greater_than_or_equal_view, &GreaterThanOrEqual::left, &GreaterThanOrEqual::right);
}

void greater_than_or_equal_destroy_sub_object_views(ObjectView *greater_than_or_equal_view) {
    generic_destroy_sub_object_views(greater_than_or_equal_view);
}