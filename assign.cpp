#include "assign.h"

ObjectViewBuilder assign_object_view_builder = ObjectViewBuilder{ASSIGN, "=", assign_create_simple, assign_create_sub_object_views, assign_destroy_sub_object_views};

void *assign_create_simple() {
    auto assign = new Assign{.grantee = nullptr, .grantor = nullptr};
    object_to_type.insert({assign, ASSIGN});
    return assign;
}

void assign_create_sub_object_views(ObjectView *assign_view) {
    generic_linear_create_views(assign_view, &Assign::grantee, &Assign::grantor);
}

void assign_destroy_sub_object_views(ObjectView *assign_view) {
    generic_destroy_sub_object_views(assign_view);
}