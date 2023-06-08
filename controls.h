#ifndef OPTO_CONTROLS_H
#define OPTO_CONTROLS_H

#include "object_view.h"
#include "lookup_box.h"

typedef struct LookupBox LookupBox; // linker sucks, need this line (for now, maybe not later)

struct KeyVisualInfo {
    int k;
    string key_name;
    Rectangle rect;
};

struct HandlerVisualInfo {
    Color color;
    string description;
};

struct ControlsView : ObjectView, Visual {
    ControlsView(LookupBox *owned_lookup_box);

    LookupBox *owned_lookup_box;

    vector<Rectangle *> hover_rectangles;
    KeyVisualInfo* hovered_key_visual_info;

    void destroy_view();

    void draw();
};

void add_hover_listener(
        function<bool(Vector2)> is_within,
        function<void(void)> hover_enter,
        function<void(void)> hover_exit
);

extern vector<KeyVisualInfo*> key_visual_infos_mac;
extern map<F*, HandlerVisualInfo> handler_visual_infos;

void add_click_listener(F *handler);

void remove_click_listener(F *handler);

void add_double_click_listener(F *handler);

void remove_double_click_listener(F *handler);

void tick_controls();

void add_key_pressed_handler(int key, F *handler);

void remove_key_pressed_handler(int key, F *handler);

void add_key_released_handler(int key, F *handler);

void remove_key_released_handler(int key, F *handler);

void add_key_pressed_handlers(const vector<KeyHandlerPair>& key_handler_pairs);

void remove_key_pressed_handlers(const vector<KeyHandlerPair>& key_handler_pairs);

void add_key_released_handlers(const vector<KeyHandlerPair>& key_handler_pairs);

void remove_key_released_handlers(const vector<KeyHandlerPair>& key_handler_pairs);

void debug_list_controls();

#endif //OPTO_CONTROLS_H
