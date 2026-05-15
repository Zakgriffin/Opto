#include "globals.h"
#include "object_view.h"
#include "object_view_temp.h"
#include "scatter_object_view.h"
#include "raylib.h"
#include "user_input.h"
#include "view_builders.h"
#include "sync.h"
#include <mutex>


ray::Font font;
int FONT_HEIGHT = 18;

ray::Color BACKGROUND_COLOR = ray::GetColor(0x111111FF);
ray::Color STRUCTURE_VIEW_COLOR = ray::GetColor(0x222222FF);
ray::Color REFERENCE_VIEW_COLOR = ray::GetColor(0x332233FF);

Signal draw_visuals;
Signal input_listeners;
int mouse_cursor;
bool mouse_clicked_during_input;
bool key_consumed;
EditMode edit_mode = ASSEMBLE_MODE;

int key_pressed = 0;

unordered_map<void *, string> tracked_objects;

auto everything = EverythingScatterObjectView(nullptr);


int main() {
    const int screen_width = 900;
    const int screen_height = 500;

    ray::SetTraceLogLevel(ray::LOG_ERROR);
    ray::SetConfigFlags(ray::FLAG_WINDOW_RESIZABLE | ray::FLAG_WINDOW_HIGHDPI);
    ray::InitWindow(screen_width, screen_height, "Opto");
    ray::SetTargetFPS(60);

    font = ray::LoadFontEx("resources/RobotoMono-Regular.ttf", FONT_HEIGHT * 2, nullptr, 250);

    ray::SetExitKey(0);


    object_to_type.insert({nullptr, NONE});
    init_object_view_builders();

    // void **recent_root;

    while (!ray::WindowShouldClose()) {
        ray::BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        // key_pressed = ray::GetKeyPressed();
        // mouse_cursor = 0;
        // mouse_clicked_during_input = false;
        // key_consumed = false;
        // signal_update(&input_listeners);
        // ray::SetMouseCursor(mouse_cursor);

        // auto click_streak = check_clicked_n_times(&multi_click, 2);
        // if (!mouse_clicked_during_input && click_streak == 2) {
        //     recent_root = new void *;
        //     // *recent_root = x;
        //     *recent_root = nullptr;
        //     auto object_view = new_object_view(recent_root);
        //     selected_editable_text = &object_view->editable_text;
        //     edit_mode = EDITABLE_TEXT;

        //     ray::Vector2 mouse = ray::GetMousePosition();
        //     move_box_x(&object_view->editable_text.box, mouse.x);
        //     move_box_y(&object_view->editable_text.box, mouse.y);
        //     signal_update(&object_view->editable_text.box_sig);
        // }

        // signal_update(&draw_visuals);

        update_input_context(&input_ctx);
        
        everything.accept_input(&input_ctx);
        everything.layout();
        everything.draw();

        ray::EndDrawing();
    }

    ray::CloseWindow();

    return 0;
}
