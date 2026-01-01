#include "integer.h"
#include <charconv>

bool to_int(const string& s, int& out) {
    auto [p, ec] = from_chars(s.data(), s.data() + s.size(), out);
    return ec == errc() && p == s.data() + s.size();
}

struct IntegerObjectView {
    EditableText integer_editable_text;
};

ObjectViewBuilder integer_object_view_builder = ObjectViewBuilder{INTEGER, "integer", integer_create_simple, integer_create_sub_object_views, integer_destroy_sub_object_views};

void *integer_create_simple() {
    auto integer = new int{0};
    object_to_type.insert({integer, INTEGER});
    return integer;
}

void integer_create_sub_object_views(ObjectView *integer_view) {
    auto integer_handle = (int **) integer_view->object_handle;
    auto integer = *integer_handle;

    auto integer_object_view = new IntegerObjectView;
    integer_view->context = integer_object_view;

    auto e = &integer_object_view->integer_editable_text;
    initialize_editable_text(e);

    integer_view->sub_object_constraints.push_back(create_listener({&integer_view->editable_text.box_sig}, new function<void(void)>([=]() {
        box_layout_right(&integer_view->editable_text.box, &e->box);
        signal_update(&e->box_sig);
    })));
    include_sub_box(integer_view, &e->box, &e->box_sig);

    e->text = to_string(*integer); // ZZZZ eh
    integer_view->sub_object_constraints.push_back(create_listener({&e->text_input_sig}, new function<void(void)>([=]() {
        int i;
        if (to_int(e->text, i)) {
            *integer = i;
            e->color = BLUE;
        } else {
            e->color = RED;
        }
    })));
    signal_update(&e->text_input_sig); // ZZZZ eh
}

void integer_destroy_sub_object_views(ObjectView *integer_view) {
    auto context = (IntegerObjectView*)integer_view->context;
    if (context) {
        finalize_editable_text(&context->integer_editable_text);
        delete context;
    }

    generic_destroy_sub_object_views(integer_view);
}