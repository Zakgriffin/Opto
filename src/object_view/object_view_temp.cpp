#include "object_view_temp.h"
#include "editable_text_temp.h"
#include "raylib.h"

ObjectViewTemp *selected_object_view_temp = nullptr;

void draw_view_box(ObjectViewTemp* v) {
    ray::Rectangle rect = box_to_rectangle(v->box);
    uintptr_t h = (uintptr_t)v * 2654435761u;
    ray::Color color = { (unsigned char)(h >> 16), (unsigned char)(h >> 8), (unsigned char)h, 255 };
    if (selected_object_view_temp == v) color = ray::WHITE;
    DrawRectangleRec(rect, color);
}

void accept_click_select(InputContext* ctx, ObjectViewTemp* v) {
    if (any_click(ctx, v->box))
        if (consume_click_streak(ctx, 1))
            selected_object_view_temp = v;
}

void layout_single_text_box(ObjectViewTemp* parent, EditableTextTemp* text_box, int nudge) {
    nudged_down_right(&text_box->box, &parent->box, nudge);
    
    text_box->layout();
    
    parent->box.x_max = text_box->box.x_max + nudge;
    parent->box.y_max = text_box->box.y_max + nudge;
}
