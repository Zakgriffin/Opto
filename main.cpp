#include <raylib.h>
#include <cstdio>
#include "globals.h"
#include "lookup_box.h"
#include "priority_queue.h"
#include "controls.h"
#include "reactivity.h"
#include "do_then.h"

using namespace std;

int main() {
    const int screen_width = 1400;
    const int screen_height = 800;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
    InitWindow(screen_width, screen_height, "Opto");
    SetTargetFPS(60);
    load_font();

    printf("Running!\n");

    auto *whole_screen_rect = new Rectangle{.x=0, .y=0, .width=screen_width, .height=screen_height};

    Fn *create_lookup_box = new Fn([]() {
        auto lookup_box = new LookupBox;

        lookup_box->on_lookup = [=](const string &text) {
            if (text == "do-then") {
                auto do_then = new DoThen{
                        .effect = nullptr,
                        .next = nullptr,
                };
                auto do_then_view = new DoThenView(do_then);
                do_then_view->lookup_box->on_lookup = [=](const string &s) {
                    delete do_then_view;
                };

                begin_data_sync();
                update_listenable(&do_then_view->lookup_box->rect.x, lookup_box->rect.x);
                update_listenable(&do_then_view->lookup_box->rect.y, lookup_box->rect.y);
                end_data_sync();

                delete lookup_box;
            }
        };

        begin_data_sync();
        Vector2 position = GetMousePosition();
        update_listenable(&lookup_box->rect.x, position.x);
        update_listenable(&lookup_box->rect.y, position.y);
        update_listenable(&lookup_box->prompt, string("opto"));
        lookup_box->select();
        end_data_sync();
    });

    auto hover_handler = new HoverHandler{
            .is_within=within_rectangle(whole_screen_rect),
            .hover_enter=[&]() {
                create_double_click_listener(create_lookup_box);
            },
            .hover_exit=[&]() {
                destroy_double_click_listener(create_lookup_box);
            },
    };
    create_hover_handler(hover_handler);


    SetExitKey(0);
    while (!WindowShouldClose()) {
        if (bindings_to_update.size() > 0) {
            printf("stuff not updated\n");
        }

        if (IsKeyPressed('`')) debug_list_globals();

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