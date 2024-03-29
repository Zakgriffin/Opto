#ifndef OPTO_GLOBALS_H
#define OPTO_GLOBALS_H

#include <unordered_set>
#include <map>
#include <functional>
#include <string>
#include "visual.h"
#include "raylib.h"
// TODO remove visual.h include

using namespace std;

class Fn {
public:
    function<void(void)> f;

    explicit Fn(function<void(void)> f);
};

struct KeyListenerPair {
    int key;
    Fn *key_listener;
};

extern unordered_set<Visual *> visuals;

extern Color BACKGROUND_COLOR;
extern Color SECONDARY_COLOR;

extern float font_width;
extern float font_height;

extern Font font;

void load_font();

float map_range(float n, float start1, float stop1, float start2, float stop2);

function<bool(Vector2)> within_rectangle(Rectangle *rect);

float center(float container_pos, float container_span, float span);

void debug_list_globals();

bool is_number(const std::string &s);

#endif //OPTO_GLOBALS_H
