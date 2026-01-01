#include "do_then.h"

ObjectViewBuilder do_then_object_view_builder = ObjectViewBuilder{DO_THEN, ",", do_then_create_simple, do_then_create_sub_object_views, do_then_destroy_sub_object_views};

void *do_then_create_simple() {
    auto do_then = new DoThen{.effect = nullptr, .next = nullptr};
    object_to_type.insert({do_then, DO_THEN});
    return do_then;
}

void do_then_create_sub_object_views(ObjectView *do_then_view) {
    auto do_then_handle = (DoThen **) do_then_view->object_handle;
    auto do_then = *do_then_handle;

    auto effect_view = new_object_view((void **) &do_then->effect);
    do_then_view->sub_object_constraints.push_back(create_listener({&do_then_view->editable_text.box_sig}, new function<void(void)>([=]() {
        box_layout_right(&do_then_view->editable_text.box, &effect_view->editable_text.box);
        signal_update(&effect_view->editable_text.box_sig);
    })));
    include_sub_object_view(do_then_view, effect_view);

    auto next_view = new_object_view((void **) &do_then->next);
    do_then_view->sub_object_constraints.push_back(create_listener({&do_then_view->editable_text.box_sig}, new function<void(void)>([=]() {
        box_layout_under(&do_then_view->editable_text.box, &next_view->editable_text.box);
        signal_update(&next_view->editable_text.box_sig);
    })));
    include_sub_object_view(do_then_view, next_view);

    // do_then_to_view.insert({do_then, do_then_view});
}

void do_then_destroy_sub_object_views(ObjectView *do_then_view) {
    // do_then_to_view.erase({(DoThen *) *do_then_view->object_handle});

    generic_destroy_sub_object_views(do_then_view);
}

string do_then_as_str(DoThen *do_then) {
    string effect_str = do_then->effect ? "something" : "null";
    string next_str = do_then->next ? do_then_as_str((DoThen*)do_then->next) : "null";
    return "do_then(" + effect_str + "," + next_str + ")";
}