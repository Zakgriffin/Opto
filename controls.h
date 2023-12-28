#ifndef OPTO_CONTROLS_H
#define OPTO_CONTROLS_H

#include "lookup_box.h"
#include "reactivity.h"

typedef struct LookupBox LookupBox; // TODO linker sucks, need this line (for now, maybe not later)

// TODO guess I wanted this here
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

struct ControlsView : Visual {
    ControlsView(LookupBox *owned_lookup_box);

    ~ControlsView();

    LookupBox *owned_lookup_box;

    vector<Rectangle *> hover_rectangles;
    KeyVisualInfo *hovered_key_visual_info;

    void draw();
};

extern vector<KeyVisualInfo *> key_visual_infos_mac;
extern map<Fn *, ListenerVisualInfo> listener_visual_infos;

void tick_controls();

void debug_list_controls();

// listeners

void create_hover_handler(HoverHandler *hover_handler);

void destroy_hover_handler(HoverHandler *hover_handler);

void create_click_listener(Fn *listener);

void destroy_click_listener(Fn *listener);

void create_double_click_listener(Fn *listener);

void destroy_double_click_listener(Fn *listener);

void create_key_pressed_listener(int key, Fn *listener);

void destroy_key_pressed_listener(int key, Fn *listener);

void create_key_released_listener(int key, Fn *listener);

void destroy_key_released_listener(int key, Fn *listener);

void create_key_pressed_listeners(const vector<KeyListenerPair> &key_listener_pairs);

void destroy_key_pressed_listeners(const vector<KeyListenerPair> &key_listener_pairs);

void create_key_released_listeners(const vector<KeyListenerPair> &key_listener_pairs);

void destroy_key_released_listeners(const vector<KeyListenerPair> &key_listener_pairs);

#endif //OPTO_CONTROLS_H
