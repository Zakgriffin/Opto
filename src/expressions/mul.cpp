#include "mul.h"
#include "object_view.h"

ObjectViewBuilder mul_object_view_builder = ObjectViewBuilder{MUL, "mul", mul_create_simple, mul_create_sub_object_views, mul_destroy_sub_object_views};

void *mul_create_simple() {
    auto mul = new Mul{.multiplicand = nullptr, .multiplier = nullptr};
    object_to_type.insert({mul, MUL});
    return mul;
}

void mul_create_sub_object_views(ObjectView *mul_view) {
    generic_linear_create_views(mul_view, &Mul::multiplicand, &Mul::multiplier);
}

void mul_destroy_sub_object_views(ObjectView *mul_view) {
    generic_destroy_sub_object_views(mul_view);
}