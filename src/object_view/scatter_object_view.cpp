#include "scatter_object_view.h"
#include "structure_object_view.h"
#include "add.h"

ScatterObjectView::ScatterObjectView(void* object) : ObjectViewTemp(object) {}

EverythingScatterObjectView::EverythingScatterObjectView(void* object) : ScatterObjectView(object) {}

void EverythingScatterObjectView::accept_input(InputContext* ctx) {
    for (auto &view : items) {
        view->accept_input(ctx);

        if (auto replacement = view->wants_replace()) {
            replacement->box.x_min = view->box.x_min;
            replacement->box.y_min = view->box.y_min;
            delete view;
            view = replacement;
        }
    }

    if (ctx->click_streak == 2) {
        auto view = new StructureObjectView(nullptr);
        auto mouse = ray::GetMousePosition();
        view->box.x_min = mouse.x;
        view->box.y_min = mouse.y;
        view->structure_text_box.selected = true;
        items.push_back(view);
        edit_mode = TEXT_MODE;
        
        ctx->click_streak = 0;
    }
}

void EverythingScatterObjectView::layout() {
    for(auto item : items) {
        item->layout();
    }
}

void EverythingScatterObjectView::draw() {
    for(auto item : items) {
        item->draw();
    }
}