#include "main.h"

Font font;
int FONT_HEIGHT = 18;

Color BACKGROUND_COLOR = GetColor(0x111111FF);
Color BOX_COLOR = GetColor(0x222222FF);

int main() {
    const int screen_width = 1400;
    const int screen_height = 800;

    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(screen_width, screen_height, "Opto");
    SetTargetFPS(60);

    font = LoadFontEx("resources/RobotoMono-Regular.ttf", FONT_HEIGHT * 2, nullptr, 250);

    double_click_listeners.insert(new Event([]() {
        auto unknown = new Unknown();
        make_top_level_object(unknown);
    }));

    SetExitKey(0);
    ugh_test();
    while (!WindowShouldClose()) {
        BeginDrawing();

//        tick_user_input();

        ClearBackground(BACKGROUND_COLOR);

        for (auto visual: visuals) {
//            visual->draw();
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
