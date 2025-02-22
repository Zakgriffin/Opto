#include "object_view.h"

vector<ObjectView *> visuals;

void box_touch_left_right(Rectangle* left_box, Rectangle* right_box) {
    // TODO
}

void box_touch_up_down(Rectangle* up_box, Rectangle* down_box) {
    // TODO
}

void box_align_left(Rectangle* box_1, Rectangle* box_2) {
    // TODO
}

void box_align_up(Rectangle* box_1, Rectangle* box_2) {
    // TODO
}

ObjectView::ObjectView(void* object) {
    /*
    subsume_lifetime_under_lifetime(this, object);
    subsume_lifetimes_under_lifetime({editable_text, box}, this);

    this->object = object;

    create_datum_update_propagated_event(&this->object, new Event([&](){
        if(object_types.includes(editable_text->text)) {
            auto type = object_types.get(text);
            Unknown* unknown = type.to_unknown(object);
            this->object = type.from_unknown(unknown);
        }
    }, {editable_text->text}));

    create_datum_listener(new Event([&](){
        create_sub_object_views(object, this);
    }, {&this->object}));

    visuals.push_back(this);
     */
}

void make_top_level_object(void* object) {
    auto object_visual = new ObjectView(object);
    Vector2 mouse = GetMousePosition();
//    assign_datum(object_visual->box.x, mouse.x);
//    assign_datum(object_visual->box.y, mouse.y);
}