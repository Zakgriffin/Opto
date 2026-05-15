#include "add.h"

#include "object.h"
#include "raylib.h"
#include "reference_object_view.h"
#include "structure_object_view.h"
#include <cstring>

int ZZZZ_nudge = 5;

// "+", Add, [fields], layout
typedef struct StructureObjectViewBuilder {
    string lookup_string;
    ObjectType type;
    void* (*create_simple)();
    vector<void**> (*get_field_slots)(void* obj);
    void (*do_layout)(StructureObjectView* view);
} StructureObjectViewBuilder;

vector<StructureObjectViewBuilder> structure_object_view_builders = {
    {
        .lookup_string = "",
        .type = NONE,
        .create_simple = []() -> void* {
            return nullptr;
        },
        .get_field_slots = [](void* obj) -> vector<void**> {
            return {};
        },
        .do_layout = [](StructureObjectView* view) -> void {
            layout_single_text_box(view, &view->structure_text_box, ZZZZ_nudge);
        }
    },
{
        .lookup_string = "+",
        .type = ADD,
        .create_simple = []() -> void* {
            auto add = typed(ADD, new Add);
            memset(add, 0, sizeof(Add));
            return add;
        },
        .get_field_slots = [](void* obj) -> vector<void**> {
            auto add = (Add*) obj;
            return {&add->addend, &add->augend};
        },
        .do_layout = [](StructureObjectView* view) -> void {
            layout_at(view, 1);
        }
    }
};

StructureObjectView::StructureObjectView(void* object) : ObjectViewTemp(object){
    this->structure_text_box.color = STRUCTURE_VIEW_COLOR;

    if(!object_to_type.contains(this->object)) {
        cout << "ZZZZ no type for object" << endl;
        abort();
    }

    auto try_builder = find_if(structure_object_view_builders.begin(), structure_object_view_builders.end(), [=](StructureObjectViewBuilder b) {
        return b.type == object_to_type[this->object];
    });
    if (try_builder == structure_object_view_builders.end()) {
        cout << "ZZZZ no builder found for type " << object_to_type[this->object] << endl;
        abort();
    }
    auto builder = *try_builder;
    this->do_layout = builder.do_layout;
    this->structure_text_box.text = builder.lookup_string;
    for (auto slot : builder.get_field_slots(object)) {
        this->field_slots.push_back(slot);
        this->field_views.push_back(new StructureObjectView(*slot));
    }

}

StructureObjectView::~StructureObjectView() {
    for (auto field_view : field_views) delete field_view;
}

void StructureObjectView::accept_input(InputContext* ctx) {
    this->structure_text_box.accept_input(ctx);

    for (int i = 0; i < (int)this->field_views.size(); i++) {
        auto &field_view = this->field_views[i];

        if(consume_if(&ctx->key, ray::KEY_DOWN, selected_object_view_temp == field_view) && ray::IsKeyDown(ray::KEY_LEFT_CONTROL)) {
            auto object = field_view->object;
            delete field_view;
            field_view = new ReferenceObjectView(object);
        }
        if(consume_if(&ctx->key, ray::KEY_UP, selected_object_view_temp == field_view) && ray::IsKeyDown(ray::KEY_LEFT_CONTROL)) {
            auto object = field_view->object;
            delete field_view;
            field_view = new StructureObjectView(object);
        }

        field_view->accept_input(ctx);

        if (auto replacement = field_view->wants_replace()) {
            *this->field_slots[i] = replacement->object;
            delete field_view;
            field_view = replacement;
        }
    }

    accept_click_select(ctx, this);

    // ZZZZ maybe not here
    if(consume_if(&ctx->key, ray::KEY_ESCAPE, edit_mode == ASSEMBLE_MODE)) {
        selected_object_view_temp = nullptr;
    }
}

static void layout_at(StructureObjectView* s, int label_pos) {
    int total = (int)s->field_views.size() + 1;
    Box* last_box = nullptr;
    int field_i = 0;

    for (int i = 0; i < total; i++) {
        Box* current_box;
        if (i == label_pos) {
            current_box = &s->structure_text_box.box;
            if (last_box) touch_x_align_top(last_box, current_box);
            else nudged_down_right(current_box, &s->box, ZZZZ_nudge);
            s->structure_text_box.layout();
        } else {
            auto field_view = s->field_views[field_i++];
            current_box = &field_view->box;
            if (last_box) touch_x_align_top(last_box, current_box);
            else nudged_down_right(current_box, &s->box, ZZZZ_nudge);
            field_view->layout();
        }
        last_box = current_box;
    }

    auto inner = s->structure_text_box.box;
    for (auto fv : s->field_views) inner = enclosing_box(inner, fv->box);
    s->box.x_max = inner.x_max + ZZZZ_nudge;
    s->box.y_max = inner.y_max + ZZZZ_nudge;
}

void StructureObjectView::layout() {
    this->do_layout(this);
}

void StructureObjectView::draw() {
    draw_view_box(this);

    this->structure_text_box.draw();

    for(auto field_view : this->field_views) {
        field_view->draw();
    }
}

ObjectViewTemp* StructureObjectView::wants_replace() {
    if(consume(&this->structure_text_box.text_signaled, 1)) {
        for(auto builder : structure_object_view_builders) {
            if(this->structure_text_box.text == builder.lookup_string) {
                auto obj = builder.create_simple();
                auto v = new StructureObjectView(obj);

                // v->do_layout = builder.do_layout;
                // for(auto field : builder.get_fields(obj)) {
                //     v->field_views.push_back(new StructureObjectView(field)); // ZZZZ this second one hints at better signaled appraoch, with consume?
                // }
        
                v->structure_text_box.selected = this->structure_text_box.selected;
                v->structure_text_box.character_index = this->structure_text_box.character_index;
                // v->structure_text_box.text = this->structure_text_box.text;
        
                return v;
            }
        }
    }
    return nullptr;
}