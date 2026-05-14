#include "object_view.h"
#include "sub.h"

ObjectViewBuilder sub_object_view_builder = ObjectViewBuilder{SUB, "sub", sub_create_simple, sub_create_sub_object_views, sub_destroy_sub_object_views};

void *sub_create_simple() {
    auto sub = new Sub{.minuend = nullptr, .subtrahend = nullptr};
    object_to_type.insert({sub, SUB});
    return sub;
}

void sub_create_sub_object_views(ObjectView *sub_view) {
    generic_linear_create_views(sub_view, &Sub::minuend, &Sub::subtrahend);
}

void sub_destroy_sub_object_views(ObjectView *sub_view) {
    generic_destroy_sub_object_views(sub_view);
}