#include "add.h"

ObjectViewBuilder add_object_view_builder = ObjectViewBuilder{ADD, "add", add_create_simple, add_create_sub_object_views, add_destroy_sub_object_views};

void *add_create_simple() {
    auto add = new Add{.augend = nullptr, .addend = nullptr};
    object_to_type.insert({add, ADD});
    return add;
}

void add_create_sub_object_views(ObjectView *add_view) {
    generic_linear_create_views(add_view, &Add::addend, &Add::augend);
}

void add_destroy_sub_object_views(ObjectView *add_view) {
    generic_destroy_sub_object_views(add_view);
}