#include "loop.h"

ObjectViewBuilder loop_object_view_builder = ObjectViewBuilder{LOOP, "loop", loop_create_simple, loop_create_sub_object_views, loop_destroy_sub_object_views};

void *loop_create_simple() {
    auto loop = new Loop{.body = nullptr};
    object_to_type.insert({loop, LOOP});
    return loop;
}

void loop_create_sub_object_views(ObjectView *loop_view) {
    auto loop_handle = (Loop **) loop_view->object_handle;
    auto loop = *loop_handle;

    auto then_view = new_object_view((void **) &loop->body);
    loop_view->sub_object_constraints.push_back(create_listener({&loop_view->editable_text.box_sig}, new function<void(void)>([=]() {
        box_layout_indent_x(&loop_view->editable_text.box, &then_view->editable_text.box);
        box_layout_under_y(&loop_view->editable_text.box, &then_view->editable_text.box);
        signal_update(&then_view->editable_text.box_sig);
    })));
    include_sub_object_view(loop_view, then_view);
}

void loop_destroy_sub_object_views(ObjectView *loop_view) {
    generic_destroy_sub_object_views(loop_view);
}