#include "string_.h"

struct StringObjectView {
    EditableText string_editable_text;
};

ObjectViewBuilder string_object_view_builder = ObjectViewBuilder{STRING, "'", string_create_simple, string_create_sub_object_views, string_destroy_sub_object_views};

void *string_create_simple() {
    auto string_ = new string("");
    object_to_type.insert({string_, STRING});
    return string_;
}

void string_create_sub_object_views(ObjectView *string_view) {
    auto string_handle = (string **) string_view->object_handle;
    auto string = *string_handle;

    auto string_object_view = new StringObjectView;
    string_view->context = string_object_view;

    auto e = &string_object_view->string_editable_text;
    initialize_editable_text(e);

    string_view->sub_object_constraints.push_back(create_listener({&string_view->editable_text.box_sig}, new function<void(void)>([=]() {
        box_layout_right(&string_view->editable_text.box, &e->box);
        signal_update(&e->box_sig);
    })));
    include_sub_box(string_view, &e->box, &e->box_sig);

    string_view->sub_object_constraints.push_back(create_listener({&e->text_input_sig}, new function<void(void)>([=]() {
        *string = e->text;
        e->color = Color(150,120,0,255);
    })));
}

void string_destroy_sub_object_views(ObjectView *string_view) {
    auto context = (StringObjectView*)string_view->context;
    finalize_editable_text(&context->string_editable_text);
    delete context;

    generic_destroy_sub_object_views(string_view);
}