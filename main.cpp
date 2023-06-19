#include <raylib.h>
#include <cstdio>
#include "globals.h"
#include "lookup_box.h"
#include "priority_queue.h"
#include "controls.h"
#include "constraint.h"
#include "unknown.h"
#include "do_then.h"

using namespace std;

DoThen test_root = {
        .effect = nullptr,
        .next = new DoThen{
                .effect = nullptr,
                .next = new DoThen{
                        .effect = nullptr,
                        .next = nullptr,
                }
        }
};

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
        auto *unknown_view = new UnknownView;
        Vector2 position = GetMousePosition();

        update_listenable(&unknown_view->primary_lookup_box->rect.x, position.x);
        update_listenable(&unknown_view->primary_lookup_box->rect.y, position.y);
        update_listenable(&unknown_view->primary_lookup_box->prompt, string("opto"));
        unknown_view->primary_lookup_box->select();
        update_all_tracked();
    });

    auto hover_listener = new HoverInfo{
            .is_within=within_rectangle(whole_screen_rect),
            .hover_enter=[&]() {
                add_double_click_listener(create_lookup_box_listener);
            },
            .hover_exit=[&]() {
                remove_double_click_listener(create_lookup_box_listener);
            },
    };
    add_hover_listener(hover_listener);


    SetExitKey(0);
    while (!WindowShouldClose()) {
        if(bindings_to_update.size() > 0) {
            printf("stuff not updated");
        }

        debug_list_globals();
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