#include "do_then.h"

void do_then_create_sub_object_views(ObjectView *do_then_view) {
//    auto do_then_handle = (DoThen **) do_then_view->object_handle;
//    auto do_then = new DoThen{.effect = nullptr, .next = nullptr};
//    *do_then_handle = do_then;
//
//    auto effect_view = new_object_view(&do_then->effect);
////    auto effect_e = &effect_view->editable_text;
//    do_then_view->sub_object_constraints.push_back(create_listener({&do_then_view->editable_text.position_sig, &do_then_view->editable_text.size_sig}, new function<void(void)>([=]() {
//        box_touch_left_right(&do_then_view->editable_text.box, &effect_view->box);
//        box_align_up(&do_then_view->editable_text.box, &effect_view->box);
//        signal_update(&effect_view->position_sig);
//    })));
//    include_sub_object_view(do_then_view, effect_view);
//
//    auto next_view = new_object_view((void **) &do_then->next);
////    auto next_e = &next_view->editable_text;
//    do_then_view->sub_object_constraints.push_back(create_listener({&do_then_view->editable_text.position_sig, &do_then_view->editable_text.size_sig}, new function<void(void)>([=]() {
//        box_align_left(&do_then_view->editable_text.box, &next_view->box);
//        box_touch_up_down(&do_then_view->editable_text.box, &next_view->box);
//        signal_update(&next_view->position_sig);
//    })));
//    include_sub_object_view(do_then_view, next_view);
}

void do_then_destroy_sub_object_views(ObjectView *do_then_view) {
    auto do_then_handle = (DoThen **) do_then_view->object_handle;
    generic_destroy_sub_object_views(do_then_view, do_then_handle);
}

void *do_then_from_unknown(Unknown *unknown) {
    return new DoThen{
            .effect = nullptr, // unknown_at(unknown, 0),
            .next = nullptr// (DoThen *) unknown_at(unknown, 1) // ZZZZ hmmm
    };
}

Unknown *do_then_to_unknown(void *object) {
    auto *do_then = (DoThen *) object;
    auto unknown = new Unknown{};
    unknown->items.push_back(do_then->effect);
    unknown->items.push_back(do_then->next);
    return unknown;
}

string do_then_as_str(DoThen *do_then) {
    string effect_str = do_then->effect ? "something" : "null";
    string next_str = do_then->next ? do_then_as_str(do_then->next) : "null";
    return "do_then(" + effect_str + "," + next_str + ")";
}