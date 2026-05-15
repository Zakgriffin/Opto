#pragma once

#include "stds.h"

typedef enum {
    ASSEMBLE_MODE,
    TEXT_MODE,
} EditMode;

typedef struct {
    float x_min;
    float x_max;
    float y_min;
    float y_max;
} Box;

ray::Rectangle box_to_rectangle(Box box);

typedef struct ObjectView ObjectView;
typedef struct ObjectViewBuilder ObjectViewBuilder;

#define VIEW_DECLARATIONS(obj) \
    extern ObjectViewBuilder obj##_object_view_builder; \
    void obj##_create_sub_object_views(ObjectView *obj##_view); \
    void obj##_destroy_sub_object_views(ObjectView *obj##_view); \
    void* obj##_create_simple();

extern ray::Color BACKGROUND_COLOR;
extern ray::Color STRUCTURE_VIEW_COLOR;
extern ray::Color REFERENCE_VIEW_COLOR;

extern ray::Font font;
extern float CHARACTER_WIDTH;

extern int key_pressed;
extern int mouse_cursor;
extern bool mouse_clicked_during_input;
extern bool key_consumed;
extern EditMode edit_mode;

struct InputContext {
    int key = 0;
    int ch = 0;
    int click_streak = 0;
    long long last_click_millis = 0;
    bool click_absorbed = false;
};

extern InputContext input_ctx;
void update_input_context(InputContext* ctx);

bool is_within_box(ray::Vector2 point, Box box);

bool consume(int* field, int value);
bool consume_if(int* field, int value, bool condition);
bool any_click(InputContext* ctx, Box box);
bool consume_click_streak(InputContext* ctx, int value);
bool consume_click_streak_if(InputContext* ctx, int value, bool condition);

void box_layout_right_x(Box *super, Box *sub);
void box_layout_flush_min_y(Box *super, Box *sub);
void box_layout_right(Box *super, Box *sub);
void box_layout_flush_min_x(Box *super, Box *sub);
void box_layout_under_y(Box *super, Box *sub);
void box_layout_under(Box *super, Box *sub);
void box_layout_right_under(Box *super, Box *sub);
void box_layout_indent_x(Box *super, Box *sub);

Box enclosing_box(Box b1, Box b2);
bool boxes_overlap(Box b1, Box b2);
Box rotate_box(Box b, int count);

void nudged_down_right(Box* child, Box* parent, int nudge);
void touch_x_align_top(Box* last, Box* next);

void move_box_x(Box* box, float x_min);

void move_box_y(Box* box, float y_min);

template<typename T>
T map_range(T n, T start1, T stop1, T start2, T stop2) {
    return ((n - start1) / (stop1 - start1)) * (stop2 - start2) + start2;
}

