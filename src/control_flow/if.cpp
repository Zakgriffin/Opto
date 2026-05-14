#include "if.h"

ObjectViewBuilder if_object_view_builder = ObjectViewBuilder{IF, "if", if_create_simple, if_create_sub_object_views, if_destroy_sub_object_views};

void *if_create_simple() {
    auto if_ = new If{.then = nullptr, .finally = nullptr};
    object_to_type.insert({if_, IF});
    return if_;
}

void if_create_sub_object_views(ObjectView *if_view) {
    auto if_handle = (If **) if_view->object_handle;
    auto if_ = *if_handle;

    auto condition_view = new_object_view((void **) &if_->condition);
    if_view->sub_object_constraints.push_back(create_listener({&if_view->editable_text.box_sig}, new function<void(void)>([=]() {
        box_layout_right(&if_view->editable_text.box, &condition_view->editable_text.box);
        signal_update(&condition_view->editable_text.box_sig);
    })));
    include_sub_object_view(if_view, condition_view);

    auto then_view = new_object_view((void **) &if_->then);
    if_view->sub_object_constraints.push_back(create_listener({&if_view->editable_text.box_sig}, new function<void(void)>([=]() {
        box_layout_right_under(&if_view->editable_text.box, &then_view->editable_text.box);
        signal_update(&then_view->editable_text.box_sig);
    })));
    include_sub_object_view(if_view, then_view);

    auto finally_view = new_object_view((void **) &if_->finally);
    if_view->sub_object_constraints.push_back(create_listener({&if_view->editable_text.box_sig, &then_view->box_sig}, new function<void(void)>([=]() {
        box_layout_flush_min_x(&if_view->editable_text.box, &finally_view->editable_text.box);
        box_layout_under_y(&then_view->box, &finally_view->editable_text.box);
        signal_update(&finally_view->editable_text.box_sig);
    })));
    include_sub_object_view(if_view, finally_view);
}

void if_destroy_sub_object_views(ObjectView *if_view) {
    generic_destroy_sub_object_views(if_view);
}