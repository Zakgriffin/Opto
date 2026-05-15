#include "globals.h"

InputContext input_ctx;

void update_input_context(InputContext* ctx) {
    ctx->key = ray::GetKeyPressed();
    ctx->ch = ray::GetCharPressed();
    ctx->click_absorbed = false;


    auto now = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch()).count();
    if (ray::IsMouseButtonPressed(0)) {
        if (ctx->click_streak > 0 && now - ctx->last_click_millis < 250LL)
            ctx->click_streak++;
        else
            ctx->click_streak = 1;
        ctx->last_click_millis = now;
    } else if (ctx->click_streak > 0 && now - ctx->last_click_millis >= 250LL) {
        ctx->click_streak = 0;
    }
}

float CHARACTER_WIDTH = 8;

bool consume(int* field, int value) {
    if (*field != value) return false;
    *field = 0;
    return true;
}

bool consume_if(int* field, int value, bool condition) {
    if (!condition) return false;
    return consume(field, value);
}

bool consume_click_streak(InputContext* ctx, int value) {
    return consume(&ctx->click_streak, value);
}

bool consume_click_streak_if(InputContext* ctx, int value, bool condition) {
    return consume_if(&ctx->click_streak, value, condition);
}

bool any_click(InputContext* ctx, Box box) {
    if (ctx->click_streak == 0) return false;
    if (ctx->click_absorbed) return false;
    if (!is_within_box(ray::GetMousePosition(), box)) return false;
    ctx->click_absorbed = true;
    return true;
}

ray::Rectangle box_to_rectangle(Box box) {
    return ray::Rectangle{
            box.x_min,
            box.y_min,
            box.x_max - box.x_min,
            box.y_max - box.y_min
    };
}

bool is_within_box(ray::Vector2 point, Box box) {
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

bool boxes_overlap(Box b1, Box b2) {
    return
        b1.x_min < b2.x_max &&
        b1.x_max > b2.x_min &&
        b1.y_min < b2.y_max &&
        b1.y_max > b2.y_min;
}

Box rotate_box(Box b, int count) {
    for (int i = 0; i < count; i++) {
        auto q = b.x_min;
        b.x_min = b.y_max;
        b.y_max = b.x_max;
        b.x_max = b.y_min;
        b.y_min = q;
    }
    return b;
}

void box_layout_right_x(Box *super, Box *sub) {
    auto x_size = sub->x_max - sub->x_min;
    sub->x_min = super->x_max;
    sub->x_max = sub->x_min + x_size;
}

void box_layout_flush_min_y(Box *super, Box *sub) {
    auto y_size = sub->y_max - sub->y_min;
    sub->y_min = super->y_min;
    sub->y_max = sub->y_min + y_size;
}

void box_layout_right(Box *super, Box *sub) {
    box_layout_right_x(super, sub);
    box_layout_flush_min_y(super, sub);
}

void box_layout_flush_min_x(Box *super, Box *sub) {
    auto x_size = sub->x_max - sub->x_min;
    sub->x_min = super->x_min;
    sub->x_max = sub->x_min + x_size;
}

void box_layout_under_y(Box *super, Box *sub) {
    auto y_size = sub->y_max - sub->y_min;
    sub->y_min = super->y_max;
    sub->y_max = sub->y_min + y_size;
}

void box_layout_under(Box *super, Box *sub) {
    box_layout_flush_min_x(super, sub);
    box_layout_under_y(super, sub);
}

void box_layout_right_under(Box *super, Box *sub) {
    box_layout_right_x(super, sub);
    box_layout_under_y(super, sub);
}

void box_layout_indent_x(Box *super, Box *sub) {
    auto x_size = sub->x_max - sub->x_min;
    sub->x_min = super->x_min + CHARACTER_WIDTH * 4;
    sub->x_max = sub->x_min + x_size;
}


void move_box_x(Box* box, float x_min) {
    auto size_x = box->x_max - box->x_min;
    box->x_min = x_min;
    box->x_max = box->x_min + size_x;
}

void nudged_down_right(Box* child, Box* parent, int nudge) {
    child->x_min = parent->x_min + nudge;
    child->y_min = parent->y_min + nudge;
}

void touch_x_align_top(Box* last, Box* next) {
    next->x_min = last->x_max;
    next->y_min = last->y_min;
}

void move_box_y(Box* box, float y_min) {
    auto size_y = box->y_max - box->y_min;
    box->y_min = y_min;
    box->y_max = box->y_min + size_y;
}