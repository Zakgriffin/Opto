#ifndef OPTO_CONTROLS_H
#define OPTO_CONTROLS_H

#include "globals.h"
#include "lookup_box.h"

struct KeyVisualInfo {
    int k;
    string key_name;
    Rectangle rect;
};

struct ListenerVisualInfo {
    Color color;
    string description;
};

extern vector<KeyVisualInfo *> key_visual_infos_mac;
extern map<Fn *, ListenerVisualInfo> listener_visual_infos;

struct ControlsView : Visual {
    ControlsView(LookupBox *owned_lookup_box);

    ~ControlsView();

    LookupBox *owned_lookup_box;

    vector<Rectangle *> hover_rectangles;
    KeyVisualInfo *hovered_key_visual_info;

    void draw() override;
};

#endif //OPTO_CONTROLS_H
