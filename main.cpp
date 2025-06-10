#include "main.h"

Font font;
int FONT_HEIGHT = 18;

Color BACKGROUND_COLOR = GetColor(0x111111FF);
Color BOX_COLOR = GetColor(0x222222FF);

Signal draw_visuals;
Signal input_listeners;
int mouse_cursor;
bool mouse_clicked_during_input;
EditMode edit_mode = OBJECT_VIEW;

int key_pressed = 0;

unordered_map<void *, string> tracked_objects;

int main() {
    const int screen_width = 900;
    const int screen_height = 500;

    SetTraceLogLevel(LOG_ERROR);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(screen_width, screen_height, "Opto");
    SetTargetFPS(60);

    font = LoadFontEx("resources/RobotoMono-Regular.ttf", FONT_HEIGHT * 2, nullptr, 250);

    SetExitKey(0);

    auto x = typed(DO_THEN, new DoThen{
            .effect = nullptr,
            .next = typed(DO_THEN, new DoThen{
                    .effect = nullptr,
                    .next = nullptr
            })
    });

    MultiClick multi_click;
    init_multi_click(&multi_click);
    init_object_view_builders();

    object_to_type.insert({nullptr, NONE});

    void **recent_root;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        key_pressed = GetKeyPressed();
        mouse_cursor = 0;
        mouse_clicked_during_input = false;
        signal_update(&input_listeners);
        SetMouseCursor(mouse_cursor);

        auto click_streak = check_clicked_n_times(&multi_click, 2);
        if (!mouse_clicked_during_input && click_streak == 2) {
            recent_root = new void *;
            *recent_root = x;
//            *recent_root = nullptr;
            auto object_view = new_object_view(recent_root);
            selected_editable_text = &object_view->editable_text;
            edit_mode = EDITABLE_TEXT;

            Vector2 mouse = GetMousePosition();
            move_box_x(&object_view->editable_text.box, mouse.x);
            move_box_y(&object_view->editable_text.box, mouse.y);
            signal_update(&object_view->editable_text.box_sig);
        }

        signal_update(&draw_visuals);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
