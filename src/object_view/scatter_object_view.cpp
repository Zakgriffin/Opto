#include "add.h"
#include "scatter_object_view.h"

ScatterObjectView::ScatterObjectView(void* object) : ObjectViewTemp(object) {}

EverythingScatterObjectView::EverythingScatterObjectView(void* object) : ScatterObjectView(object) {}

void EverythingScatterObjectView::accept_input(InputContext* ctx) {
    for (auto &item_view : items) {
        item_view->accept_input(ctx);
        
        auto replacement = item_view->wants_replace();
        if (replacement) {
            replacement->box.x_min = item_view->box.x_min;
            replacement->box.y_min = item_view->box.y_min;
            
            delete item_view;
            item_view = replacement;
        }
    }

    if (consume_click_streak(ctx, 2)) {
        auto view = new StructureObjectView(nullptr);
        auto mouse = ray::GetMousePosition();
        view->box.x_min = mouse.x;
        view->box.y_min = mouse.y;
        view->structure_text_box.selected = true;
        items.push_back(view);
        edit_mode = TEXT_MODE;
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