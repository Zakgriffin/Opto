#include "repeat.h"

ObjectViewBuilder repeat_object_view_builder = ObjectViewBuilder{REPEAT, "repeat", repeat_create_simple, repeat_create_sub_object_views, repeat_destroy_sub_object_views};

void *repeat_create_simple() {
    auto repeat = new Repeat{.then = nullptr, .finally = nullptr};
    object_to_type.insert({repeat, REPEAT});
    return repeat;
}

void repeat_create_sub_object_views(ObjectView *repeat_view) {
    auto repeat_handle = (Repeat **) repeat_view->object_handle;
    auto repeat = *repeat_handle;

    auto then_view = new_object_view((void **) &repeat->then);
    repeat_view->sub_object_constraints.push_back(create_listener({&repeat_view->editable_text.box_sig}, new function<void(void)>([=]() {
        box_layout_indent_x(&repeat_view->editable_text.box, &then_view->editable_text.box);
        box_layout_under_y(&repeat_view->editable_text.box, &then_view->editable_text.box);
        signal_update(&then_view->editable_text.box_sig);
    })));
    include_sub_object_view(repeat_view, then_view);

    auto condition_view = new_object_view((void **) &repeat->condition);
    repeat_view->sub_object_constraints.push_back(create_listener({&then_view->box_sig, &repeat_view->editable_text.box_sig}, new function<void(void)>([=]() {
        box_layout_under_y(&then_view->box, &condition_view->editable_text.box);
        box_layout_flush_min_x(&repeat_view->editable_text.box, &condition_view->editable_text.box);
        signal_update(&condition_view->editable_text.box_sig);
    })));
    include_sub_object_view(repeat_view, condition_view);

    auto finally_view = new_object_view((void **) &repeat->finally);
    repeat_view->sub_object_constraints.push_back(create_listener({&condition_view->box_sig, &then_view->box_sig}, new function<void(void)>([=]() {
        box_layout_under(&condition_view->box, &finally_view->editable_text.box);
        signal_update(&finally_view->editable_text.box_sig);
    })));
    include_sub_object_view(repeat_view, finally_view);
}

void repeat_destroy_sub_object_views(ObjectView *repeat_view) {
    generic_destroy_sub_object_views(repeat_view);
}