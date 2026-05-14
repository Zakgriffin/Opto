#include "object_view.h"
#include "variable.h"

ObjectViewBuilder variable_object_view_builder = ObjectViewBuilder{VARIABLE, "var", variable_create_simple, variable_create_sub_object_views, variable_destroy_sub_object_views};

void *variable_create_simple() {
    auto variable = new Variable;
    object_to_type.insert({variable, VARIABLE});
    return variable;
}

void variable_create_sub_object_views(ObjectView *variable_view) {
    // generic_linear_create_views(variable_view);
}

void variable_destroy_sub_object_views(ObjectView *variable_view) {
    // generic_destroy_sub_object_views(variable_view);
}
