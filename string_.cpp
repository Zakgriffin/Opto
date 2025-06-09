#include "string_.h"

struct StringObjectView {
    EditableText string_editable_text;
};

void string_create_sub_object_views(ObjectView *string_view) {
//    auto string_handle = (String_ **) string_view->object_handle;
//    auto string = new String_{.s = ""};
//    *string_handle = string;
//
//    auto string_object_view_handle = (StringObjectView **) &string_view->context;
//    auto string_object_view = new StringObjectView;
//    *string_object_view_handle = string_object_view;
//
//    auto e = &string_object_view->string_editable_text;
//    initialize_editable_text(e);
//    e->color = BLUE;
//
//    string_view->sub_object_constraints.push_back(create_listener({&string_view->position_sig, &string_view->size_sig}, new function<void(void)>([=]() {
//        box_touch_left_right(&string_view->box, &e->box);
//        box_align_up(&string_view->box, &e->box);
//    })));
//
//    string_view->sub_object_constraints.push_back(create_listener({&e->text_sig}, new function<void(void)>([=]() {
//        string->s = e->text;
//    })));
}

void string_destroy_sub_object_views(ObjectView *string_view) {
//    auto string_handle = (String_ **) string_view->object_handle;
//    auto string_object_view_handle = (StringObjectView **) &string_view->context;
//    auto string_object_view = *string_object_view_handle;
//
//    generic_destroy_sub_object_views(string_view, string_object_view_handle);
//    finalize_editable_text(&string_object_view->string_editable_text);
//
//    delete string_object_view;
}