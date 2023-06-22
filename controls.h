#ifndef OPTO_CONTROLS_H
#define OPTO_CONTROLS_H

#include "object_view.h"
#include "lookup_box.h"
#include "constraint.h"

typedef struct LookupBox LookupBox; // linker sucks, need this line (for now, maybe not later)

//struct HoverInfo {
//    function<bool(Vector2)> is_within;
//    function<void(void)> hover_enter;
//    function<void(void)> hover_exit;
//};

struct KeyVisualInfo {
    int k;
    string key_name;
    Rectangle rect;
};

struct ListenerVisualInfo {
    Color color;
    string description;
};

struct ControlsView : ObjectView, Visual {
    ControlsView(LookupBox *owned_lookup_box);

    ~ControlsView();

    CleanupContext c;

    LookupBox *owned_lookup_box;

    vector<Rectangle *> hover_rectangles;
    KeyVisualInfo *hovered_key_visual_info;

    void draw();
};

extern vector<KeyVisualInfo *> key_visual_infos_mac;
extern map<F *, ListenerVisualInfo> listener_visual_infos;

void tick_controls();

void debug_list_controls();

// listeners

void create_hover_listener(HoverInfo *hover_info);

void destroy_hover_listener(HoverInfo *hover_info);

void create_click_listener(F *listener);

void destroy_click_listener(F *listener);

void create_double_click_listener(F *listener);

void destroy_double_click_listener(F *listener);

void create_key_pressed_listener(int key, F *listener);

void remove_key_pressed_listener(int key, F *listener);

void create_key_released_listener(int key, F *listener);

void destroy_key_released_listener(int key, F *listener);

void create_key_pressed_listeners(const vector<KeyListenerPair> &key_listener_pairs);

void destroy_key_pressed_listeners(const vector<KeyListenerPair> &key_listener_pairs);

void create_key_released_listeners(const vector<KeyListenerPair> &key_listener_pairs);

void destroy_key_released_listeners(const vector<KeyListenerPair> &key_listener_pairs);

#endif //OPTO_CONTROLS_H
