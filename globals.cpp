#include "globals.h"

#include "raylib.h"

#include "controls.h"
#include "constraint.h"

using namespace std;

map<void*, string> debug_names;

unordered_set<Visual *> visuals;

Color BACKGROUND_COLOR = GetColor(0x111111FF);
Color SECONDARY_COLOR = GetColor(0x222222FF);

float font_width = 8;
float font_height = 18;

Font font;

void load_font() {
    font = LoadFontEx("resources/RobotoMono-Regular.ttf", font_height * 2, nullptr, 250);
}

float map_range(float n, float start1, float stop1, float start2, float stop2) {
    return (n - start1) / (stop1 - start1) * (stop2 - start2) + start2;
}

function<bool(Vector2)> within_rectangle(Rectangle* rect) {
    return [=](Vector2 point){return CheckCollisionPointRec( point,  *rect); };
}

F::F(function<void(void)> f) {
    this->f = f;
};

float center(float container_pos, float container_span, float span) {
    float container_center_pos = container_pos + container_span / 2;
    return container_center_pos - span / 2;
}

void debug_list_globals() {
    debug_list_constraint();
    debug_list_controls();
    printf("\n");
}