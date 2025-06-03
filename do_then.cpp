#include "do_then.h"

void do_then_create_sub_object_views(ObjectView *do_then_view) {
    auto do_then_handle = (DoThen **) do_then_view->object_handle;
    auto do_then = new DoThen{.effect = nullptr, .next = nullptr};
    *do_then_handle = do_then;

    auto effect_view = new_object_view(&do_then->effect, UNKNOWN);
    do_then_view->sub_object_constraints.push_back(create_listener({&do_then_view->position_sig, &do_then_view->size_sig}, new function<void(void)>([=]() {
        box_touch_left_right(&do_then_view->box, &effect_view->box);
        box_align_up(&do_then_view->box, &effect_view->box);
        signal_update(&effect_view->position_sig);
    })));
    do_then_view->sub_object_views.insert(effect_view);

    auto next_view = new_object_view((void **) &do_then->next, DO_THEN);
    do_then_view->sub_object_constraints.push_back(create_listener({&do_then_view->position_sig, &do_then_view->size_sig}, new function<void(void)>([=]() {
        box_align_left(&do_then_view->box, &next_view->box);
        box_touch_up_down(&do_then_view->box, &next_view->box);
        signal_update(&next_view->position_sig);
    })));
    do_then_view->sub_object_views.insert(next_view);
}

void do_then_destroy_sub_object_views(ObjectView *do_then_view) {
    auto do_then_handle = (DoThen **) do_then_view->object_handle;
    for (const auto &sub_object_constraint: do_then_view->sub_object_constraints) {
        destroy_listener(sub_object_constraint);
    }
    for (auto sub: do_then_view->sub_object_views) {
        destroy_object_view(sub);
        // maybe rather than destroy object view controlling data
        // thats now being used for something else,
        // transfer sub objects to new object view
    }
    delete *do_then_handle;

    do_then_view->sub_object_constraints.clear();
    do_then_view->sub_object_views.clear();
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