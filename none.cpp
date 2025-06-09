#include "none.h"

ObjectViewBuilder none_object_view_builder = ObjectViewBuilder{NONE, "none", none_create_simple, none_create_sub_object_views, none_destroy_sub_object_views};

void *none_create_simple() {
    return nullptr;
}

void none_create_sub_object_views(ObjectView *none_view) {
}

void none_destroy_sub_object_views(ObjectView *none_view) {
}