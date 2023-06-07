#include "raylib.h"

using namespace std;

struct ClickHandler {
    function<void(void)> on_click;
    function<bool(Vector2)> is_hovered();
};

void is_hovered_rectangle() {

}