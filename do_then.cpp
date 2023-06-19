#include "do_then.h"

#include <vector>
#include "lookup_box.h"
#include "constraint.h"

using namespace std;

DoThenView::DoThenView(DoThen *do_then_in) {
    do_then = do_then_in;

    next_lookup_box = nullptr;
    next_object_view = nullptr;

    next_on_lookup = [&](string text) {
        if (text == "d") {
            update_listenable(&do_then->next, new DoThen{
                    .effect = nullptr,
                    .next = nullptr
            });
        }
    };

    owned_lookup_box = new LookupBox;
    update_listenable(&owned_lookup_box->text, string("do_then"));

    next_change_listener = new F([&]() {
        if (do_then->next) {
            delete next_lookup_box;
            next_lookup_box = nullptr;

            delete next_object_view;
            next_object_view = new DoThenView(do_then->next);
//            next_object_view->owned_lookup_box->on_lookup = next_on_lookup;

            next_object_view->c.make_binding(&next_object_view->owned_lookup_box->rect.x, {&owned_lookup_box->rect.x},
                                             [&]() {
                                                 next_object_view->owned_lookup_box->rect.x = owned_lookup_box->rect.x;
                                             });

            next_object_view->c.make_binding(&next_object_view->owned_lookup_box->rect.y,
                                             {&owned_lookup_box->rect.y, &owned_lookup_box->rect.height}, [&]() {
                        next_object_view->owned_lookup_box->rect.y =
                                owned_lookup_box->rect.y + owned_lookup_box->rect.height;
                    });
            update_all_tracked();
        } else {
            next_lookup_box = new LookupBox;
            update_listenable(&next_lookup_box->prompt, string("next"));

            next_lookup_box->c.make_binding(&next_lookup_box->rect.x, {&owned_lookup_box->rect.x}, [&]() {
                next_lookup_box->rect.x = owned_lookup_box->rect.x;
            });

            next_lookup_box->c.make_binding(&next_lookup_box->rect.y,
                                            {&owned_lookup_box->rect.y, &owned_lookup_box->rect.height}, [&]() {
                        next_lookup_box->rect.y = owned_lookup_box->rect.y + owned_lookup_box->rect.height;
                    });
            update_all_tracked();
            next_lookup_box->on_lookup = next_on_lookup;
        }
    });
    c.make_listener(&do_then->next, next_change_listener);
}

DoThenView::~DoThenView() {
    c.destroy_all();

    delete owned_lookup_box;

    delete next_lookup_box;
    delete next_object_view;
}
