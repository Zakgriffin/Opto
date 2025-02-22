#include "do_then.h"

void create_sub_object_views_for_do_then(ObjectView *do_then_view) {
    auto do_then = (DoThen *) &do_then_view->object;

    auto effect_view = new ObjectView(do_then->effect);
    box_touch_left_right(&do_then_view->box, &effect_view->box);
    box_align_up(&do_then_view->box, &effect_view->box);

    auto next_view = new ObjectView(do_then->next);
    box_align_left(&do_then_view->box, &next_view->box);
    box_touch_up_down(&do_then_view->box, &next_view->box);
}