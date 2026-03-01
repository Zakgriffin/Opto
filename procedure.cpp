#include "procedure.h"

ObjectViewBuilder procedure_object_view_builder = ObjectViewBuilder{PROCEDURE, "procedure", procedure_create_simple, procedure_create_sub_object_views, procedure_destroy_sub_object_views};

void *procedure_create_simple() {
    auto procedure = new Procedure{.param = nullptr, .body = nullptr};
    object_to_type.insert({procedure, PROCEDURE});
    return procedure;
}

void procedure_create_sub_object_views(ObjectView *procedure_view) {
    auto procedure_handle = (Procedure **) procedure_view->object_handle;
    auto procedure = *procedure_handle;

    auto param_view = new_object_view((void **) &procedure->param);
    procedure_view->sub_object_constraints.push_back(create_listener({&procedure_view->editable_text.box_sig}, new function<void(void)>([=]() {
        box_layout_right(&procedure_view->editable_text.box, &param_view->editable_text.box);
        signal_update(&param_view->editable_text.box_sig);
    })));
    include_sub_object_view(procedure_view, param_view);

    auto body_view = new_object_view((void **) &procedure->body);
    procedure_view->sub_object_constraints.push_back(create_listener({&procedure_view->editable_text.box_sig}, new function<void(void)>([=]() {
        box_layout_right_under(&procedure_view->editable_text.box, &body_view->editable_text.box);
        signal_update(&body_view->editable_text.box_sig);
    })));
    include_sub_object_view(procedure_view, body_view);
}

void procedure_destroy_sub_object_views(ObjectView *procedure_view) {
    generic_destroy_sub_object_views(procedure_view);
}