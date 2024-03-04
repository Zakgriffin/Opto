#include "raylib.h"
#include "has_bounding_box.h"
#include "globals.h"
#include "reactivity.h"

void align_below(Rectangle *base, Rectangle *below) {
    create_binding(&below->x, {&base->x}, [=]() {
        below->x = base->x;
    });
    create_binding(&below->y, {&base->y, &base->height}, [=]() {
        below->y = base->y + base->height;
    });
}

void align_right(Rectangle *base, Rectangle *right) {
    create_binding(&right->x, {&base->x, &base->width}, [=]() {
        right->x = base->x + base->width;
    });
    create_binding(&right->y, {&base->y}, [=]() {
        right->y = base->y;
    });
}

void detach_aligned(Rectangle *rect) {
    destroy_binding(&rect->x);
    destroy_binding(&rect->y);
}