#include "structure_object_view.h"
#include "editable_text_temp.h"


#include "add.h"
#include "globals.h"
#include "object_view_temp.h"
#include "raylib.h"
#include "reference_object_view.h"

int ZZZZ_nudge = 5;

StructureObjectView::StructureObjectView(void* object) : ObjectViewTemp(object){
    this->structure_text_box = EditableTextTemp();
}

void StructureObjectView::accept_input(InputContext* ctx) {
    this->structure_text_box.accept_input(ctx);

    for (auto &view : this->field_views) {
        if(consume_if(&ctx->key, ray::KEY_DOWN, selected_object_view_temp == view)) {
            auto object = view->object;
            delete view;
            view = new ReferenceObjectView(object);
        }

        view->accept_input(ctx);

        if (auto replacement = view->wants_replace()) {
            delete view;
            view = replacement;
        }
    }

    if (any_click(ctx, this->box)) {
        if(consume_click_streak(ctx, 1)) {
            selected_object_view_temp = this;
        }
    }

    // ZZZZ maybe not here
    if(consume_if(&ctx->key, ray::KEY_ESCAPE, edit_mode == ASSEMBLE_MODE)) {
        selected_object_view_temp = nullptr;
    }
}


static void close_layout(StructureObjectView* s) {
    auto inner = s->structure_text_box.box;
    for (auto fv : s->field_views) inner = enclosing_box(inner, fv->box);
    s->box.x_max = inner.x_max + ZZZZ_nudge;
    s->box.y_max = inner.y_max + ZZZZ_nudge;
}

static void layout_at(StructureObjectView* s, int label_pos) {
    int total = (int)s->field_views.size() + 1;
    Box* last = nullptr;
    int fi = 0;

    for (int i = 0; i < total; i++) {
        Box* cur;
        if (i == label_pos) {
            cur = &s->structure_text_box.box;
            if (last) touch_x_align_top(last, cur); else nudged_down_right(cur, &s->box, ZZZZ_nudge);
            s->structure_text_box.layout();
        } else {
            auto fv = s->field_views[fi++];
            cur = &fv->box;
            if (last) touch_x_align_top(last, cur); else nudged_down_right(cur, &s->box, ZZZZ_nudge);
            fv->layout();
        }
        last = cur;
    }

    close_layout(s);
}

void StructureObjectView::layout() {
    layout_at(this, 0);
}

void StructureObjectView::draw() {
    ray::Rectangle rect = box_to_rectangle(this->box);
    uintptr_t h = (uintptr_t)this * 2654435761u;
    ray::Color color = { (unsigned char)(h >> 16), (unsigned char)(h >> 8), (unsigned char)h, 255 };
    if(selected_object_view_temp == this) {
        color = ray::WHITE;
    }
    DrawRectangleRec(rect, color);

    this->structure_text_box.draw();

    for(auto field_view : this->field_views) {
        field_view->draw();
    }
}

ObjectViewTemp* StructureObjectView::wants_replace() {
    if(this->structure_text_box.text == "+") {
        auto add = new Add;
        auto v = new AddStructureObjectView(add);

        v->structure_text_box.selected = this->structure_text_box.selected;
        v->structure_text_box.text = this->structure_text_box.text;
        v->structure_text_box.character_index = this->structure_text_box.character_index;

        return v;
    }
    return nullptr;
}



AddStructureObjectView::AddStructureObjectView(void* add) : StructureObjectView(add) {
    this->field_views.push_back(new StructureObjectView(nullptr));
    this->field_views.push_back(new StructureObjectView(nullptr));
}

ObjectViewTemp* AddStructureObjectView::wants_replace() {
    return nullptr;
}

void AddStructureObjectView::layout() {
    layout_at(this, 1);
}