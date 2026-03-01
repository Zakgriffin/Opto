#include "call.h"

ObjectViewBuilder call_object_view_builder = ObjectViewBuilder{CALL, "call", call_create_simple, call_create_sub_object_views, call_destroy_sub_object_views};

void *call_create_simple() {
    auto call = new Call{.procedure = nullptr, .param = nullptr};
    object_to_type.insert({call, CALL});
    return call;
}

void call_create_sub_object_views(ObjectView *call_view) {
    generic_linear_create_views(call_view, &Call::procedure, &Call::param);
}

void call_destroy_sub_object_views(ObjectView *call_view) {
    generic_destroy_sub_object_views(call_view);
}