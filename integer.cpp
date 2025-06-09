#include "integer.h"

struct IntegerObjectView {
    EditableText integer_editable_text;
};

void integer_create_sub_object_views(ObjectView *integer_view) {
//    auto integer_handle = (Integer **) integer_view->object_handle;
//    auto integer = new Integer{.i = 0};
//    *integer_handle = integer;
//
//    auto integer_object_view_handle = (IntegerObjectView **) &integer_view->context;
//    auto integer_object_view = new IntegerObjectView;
//    *integer_object_view_handle = integer_object_view;
//
//    auto e = &integer_object_view->integer_editable_text;
//    initialize_editable_text(e);
//    e->color = PURPLE;
//
//    integer_view->sub_object_constraints.push_back(create_listener({&integer_view->position_sig, &integer_view->size_sig}, new function<void(void)>([=]() {
//        box_touch_left_right(&integer_view->box, &e->box);
//        box_align_up(&integer_view->box, &e->box);
//    })));
//
//    integer_view->sub_object_constraints.push_back(create_listener({&e->text_sig}, new function<void(void)>([=]() {
//        integer->i = 69;
//    })));
}

void integer_destroy_sub_object_views(ObjectView *integer_view) {
//    auto integer_handle = (Integer **) integer_view->object_handle;
//    auto integer_object_view_handle = (IntegerObjectView **) &integer_view->context;
//    auto integer_object_view = *integer_object_view_handle;
//
//    generic_destroy_sub_object_views(integer_view, integer_handle);
//    finalize_editable_text(&integer_object_view->integer_editable_text);
//
//    delete integer_object_view;
}