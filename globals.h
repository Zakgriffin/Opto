#ifndef OPTO_GLOBALS_H
#define OPTO_GLOBALS_H

#include <unordered_set>
#include <map>
#include "visual.h"
#include "raylib.h"

using namespace std;

class F {
public:
    function<void(void)> f;
    explicit F(function<void(void)> f);
};

struct KeyHandlerPair {
    int key;
    F *key_handler;
};

extern unordered_set<Visual *> visuals;

extern Color BACKGROUND_COLOR;
extern Color SECONDARY_COLOR;

extern float font_width;
extern float font_height;

extern Font font;

void init_globals();

float map_range(float n, float start1, float stop1, float start2, float stop2);

function<bool(Vector2)> within_rectangle(Rectangle *rect);

float center(float container_pos, float container_span, float span);

#endif //OPTO_GLOBALS_H
