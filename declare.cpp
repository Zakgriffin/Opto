#include "declare.h"

ObjectViewBuilder declare_object_view_builder = ObjectViewBuilder{DECLARE, "var", declare_create_simple, declare_create_sub_object_views, declare_destroy_sub_object_views};

void *declare_create_simple() {
    auto declare = new Declare{.name = nullptr};
    object_to_type.insert({declare, DECLARE});
    return declare;
}

void declare_create_sub_object_views(ObjectView *declare_view) {
    generic_linear_create_views(declare_view, &Declare::name);
}

void declare_destroy_sub_object_views(ObjectView *declare_view) {
    generic_destroy_sub_object_views(declare_view);
}