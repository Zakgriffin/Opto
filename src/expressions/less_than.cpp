#include "object_view.h"
#include "less_than.h"

ObjectViewBuilder less_than_object_view_builder = ObjectViewBuilder{LESS_THAN, "lt", less_than_create_simple, less_than_create_sub_object_views, less_than_destroy_sub_object_views};

void *less_than_create_simple() {
    auto less_than = new LessThan{.left = nullptr, .right = nullptr};
    object_to_type.insert({less_than, LESS_THAN});
    return less_than;
}

void less_than_create_sub_object_views(ObjectView *less_than_view) {
    generic_linear_create_views(less_than_view, &LessThan::left, &LessThan::right);
}

void less_than_destroy_sub_object_views(ObjectView *less_than_view) {
    generic_destroy_sub_object_views(less_than_view);
}