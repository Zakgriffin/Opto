#include "and.h"

ObjectViewBuilder and_object_view_builder = ObjectViewBuilder{AND, "and", and_create_simple, and_create_sub_object_views, and_destroy_sub_object_views};

void *and_create_simple() {
    auto and_ = new And{.conjunct_1 = nullptr, .conjunct_2 = nullptr};
    object_to_type.insert({and_, AND});
    return and_;
}

void and_create_sub_object_views(ObjectView *and_view) {
    generic_linear_create_views(and_view, &And::conjunct_1, &And::conjunct_2);
}

void and_destroy_sub_object_views(ObjectView *and_view) {
    generic_destroy_sub_object_views(and_view);
}