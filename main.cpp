#include <raylib.h>
#include <cstdio>
#include "globals.h"
#include "lookup_box.h"
#include "priority_queue.h"
#include "controls.h"
#include "constraint.h"

using namespace std;

int main() {
//    constraint_testing();

    const int screen_width = 1400;
    const int screen_height = 800;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(screen_width, screen_height, "Opto");

    SetTargetFPS(60);

    load_font();

    printf("Running!\n");

    auto *whole_screen_rect = new Rectangle{.x=0, .y=0, .width=screen_width, .height=screen_height};

    F *create_lookup_box_listener = new F([]() {
        auto *lookup_box = new LookupBox("opto");
        Vector2 position = GetMousePosition();

        update_listenable(&lookup_box->rect.x, position.x);
        update_listenable(&lookup_box->rect.y, position.y);
        lookup_box->select();

        visuals.insert(lookup_box);
    });

    add_hover_listener(within_rectangle(whole_screen_rect), [&]() {
        add_double_click_listener(create_lookup_box_listener);
    }, [&]() {
        remove_double_click_listener(create_lookup_box_listener);
    });


    SetExitKey(0);
    while (!WindowShouldClose()) {
//        debug_list_globals();

        tick_controls();

        BeginDrawing();

        ClearBackground(BACKGROUND_COLOR);

        for (Visual *visual: visuals) {
            visual->draw();
        }

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();

    return 0;
}