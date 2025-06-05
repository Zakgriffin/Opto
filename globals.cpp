#include "globals.h"

Rectangle box_to_rectangle(Box box) {
    return Rectangle{
            box.x_min,
            box.y_min,
            box.x_max - box.x_min,
            box.y_max - box.y_min
    };
}

bool is_within_box(Vector2 point, Box box) {
    return (
            point.x > box.x_min &&
            point.x < box.x_max &&
            point.y > box.y_min &&
            point.y < box.y_max
    );
}

Box enclosing_box(Box b1, Box b2) {
    auto x_min = b1.x_min < b2.x_min ? b1.x_min : b2.x_min;
    auto x_max = b1.x_max > b2.x_max ? b1.x_max : b2.x_max;
    auto y_min = b1.y_min < b2.y_min ? b1.y_min : b2.y_min;
    auto y_max = b1.y_max > b2.y_max ? b1.y_max : b2.y_max;

    return Box{x_min, x_max, y_min, y_max};
}

void box_layout_right(Box *super, Box *sub, float temp_nudge) {
    auto x_size = sub->x_max - sub->x_min;
    sub->x_min = super->x_max;
    sub->x_max = sub->x_min + x_size;

    auto y_size = sub->y_max - sub->y_min;
    sub->y_min = super->y_min + temp_nudge;
    sub->y_max = sub->y_min + y_size;
}

void move_box_x(Box* box, float x_min) {
    auto size_x = box->x_max - box->x_min;
    box->x_min = x_min;
    box->x_max = box->x_min + size_x;
}

void move_box_y(Box* box, float y_min) {
    auto size_y = box->y_max - box->y_min;
    box->y_min = y_min;
    box->y_max = box->y_min + size_y;
}