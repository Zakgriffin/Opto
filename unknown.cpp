#include "unknown.h"

#include "constraint.h"
#include "do_then.h"

UnknownView::UnknownView() {
    primary_lookup_box = new LookupBox;

    primary_lookup_box->on_lookup = [&](string text) {
        if (text == "do-then") {
            auto do_then_object_view = new DoThenView(new DoThen{
                    .effect = nullptr,
                    .next = nullptr
            }, primary_lookup_box);

            update_listenable(
                    &do_then_object_view->owned_lookup_box->rect.x,
                    primary_lookup_box->rect.x
            );
            update_listenable(
                    &do_then_object_view->owned_lookup_box->rect.y,
                    primary_lookup_box->rect.y
            );
            update_all_tracked();

            delete this;
        }
    };
}

UnknownView::~UnknownView() {
//    delete primary_lookup_box;
}
