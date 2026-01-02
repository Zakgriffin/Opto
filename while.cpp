#include "while.h"

ObjectViewBuilder while_object_view_builder = ObjectViewBuilder{WHILE, "while", while_create_simple, while_create_sub_object_views, while_destroy_sub_object_views};

void *while_create_simple() {
    auto while_ = new While{.then = nullptr, .finally = nullptr};
    object_to_type.insert({while_, WHILE});
    return while_;
}

void while_create_sub_object_views(ObjectView *while_view) {
    auto while_handle = (While **) while_view->object_handle;
    auto while_ = *while_handle;

    auto condition_view = new_object_view((void **) &while_->condition);
    while_view->sub_object_constraints.push_back(create_listener({&while_view->editable_text.box_sig}, new function<void(void)>([=]() {
        box_layout_right(&while_view->editable_text.box, &condition_view->editable_text.box);
        signal_update(&condition_view->editable_text.box_sig);
    })));
    include_sub_object_view(while_view, condition_view);

    auto then_view = new_object_view((void **) &while_->then);
    while_view->sub_object_constraints.push_back(create_listener({&condition_view->box_sig}, new function<void(void)>([=]() {
        box_layout_indent_x(&while_view->editable_text.box, &then_view->editable_text.box);
        box_layout_under_y(&condition_view->box, &then_view->editable_text.box);
        signal_update(&then_view->editable_text.box_sig);
    })));
    include_sub_object_view(while_view, then_view);

    auto finally_view = new_object_view((void **) &while_->finally);
    while_view->sub_object_constraints.push_back(create_listener({&while_view->editable_text.box_sig, &then_view->box_sig}, new function<void(void)>([=]() {
        box_layout_flush_min_x(&while_view->editable_text.box, &finally_view->editable_text.box);
        box_layout_under_y(&then_view->box, &finally_view->editable_text.box);
        signal_update(&finally_view->editable_text.box_sig);
    })));
    include_sub_object_view(while_view, finally_view);
}

void while_destroy_sub_object_views(ObjectView *while_view) {
    generic_destroy_sub_object_views(while_view);
}