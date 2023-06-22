#include "do_then.h"

#include <vector>
#include "lookup_box.h"
#include "constraint.h"

using namespace std;

DoThenView::DoThenView(DoThen *do_then_in, LookupBox *owned_lookup_box_in) {
    do_then = do_then_in;

    owned_lookup_box = owned_lookup_box_in;
    update_listenable(&owned_lookup_box->text, string("do-then"));

    // next

    next_lookup_box = nullptr;
    next_object_view = nullptr;

    next_on_lookup = [&](string text) {
        if (text == "do-then") {
            update_listenable(&do_then->next, new DoThen{
                    .effect = nullptr,
                    .next = nullptr
            });
        } else if (do_then->next) {
            update_listenable(&do_then->next, (DoThen *) nullptr);
        }
    };

    next_change_listener = new F([&]() {
        delete next_lookup_box;
        delete next_object_view;
        if (do_then->next) {
            next_lookup_box = nullptr;
            next_object_view = new DoThenView(do_then->next, next_lookup_box ? next_lookup_box : new LookupBox);

            next_object_view->owned_lookup_box->on_lookup = next_on_lookup;

            next_object_view->c.make_binding(
                    &next_object_view->owned_lookup_box->rect.x,
                    {&owned_lookup_box->rect.x},
                    [&]() {
                        next_object_view->owned_lookup_box->rect.x = owned_lookup_box->rect.x;
                    });


            next_object_view->c.make_binding(
                    &next_object_view->owned_lookup_box->rect.y,
                    {&owned_lookup_box->rect.y, &owned_lookup_box->rect.height},
                    [&]() {
                        next_object_view->owned_lookup_box->rect.y =
                                owned_lookup_box->rect.y + owned_lookup_box->rect.height;
                    });
        } else {
            next_object_view = nullptr;
            next_lookup_box = new LookupBox;

            update_listenable(&next_lookup_box->prompt, string("next"));

            next_lookup_box->c.make_binding(&next_lookup_box->rect.x, {&owned_lookup_box->rect.x}, [&]() {
                next_lookup_box->rect.x = owned_lookup_box->rect.x;
            });

            next_lookup_box->c.make_binding(
                    &next_lookup_box->rect.y,
                    {&owned_lookup_box->rect.y, &owned_lookup_box->rect.height},
                    [&]() {
                        next_lookup_box->rect.y = owned_lookup_box->rect.y + owned_lookup_box->rect.height;
                    });
            next_lookup_box->on_lookup = next_on_lookup;
        }
        update_all_tracked();
    });
    c.make_listener(&do_then->next, next_change_listener);

    // effect

//    effect_lookup_box = nullptr;
//    effect_object_view = nullptr;
//
//    effect_on_lookup = [&](string text) {
//        if (text == "1") {
//            update_listenable(&do_then->effect, (void *) new int{1});
//        } else if (do_then->effect) {
//            update_listenable(&do_then->effect, (void *) nullptr);
//        }
//    };
//
//    effect_change_listener = new F([&]() {
//        if (do_then->effect) {
//            delete effect_lookup_box;
//            effect_lookup_box = nullptr;
//
//            delete effect_object_view;
//            effect_object_view = new ObjectView(do_then->effect);
//            effect_object_view->owned_lookup_box->on_lookup = effect_on_lookup;
//
//            effect_object_view->c.make_binding(&effect_object_view->owned_lookup_box->rect.x,
//                                               {&owned_lookup_box->rect.x},
//                                               [&]() { effect_object_view->owned_lookup_box->rect.x = owned_lookup_box->rect.x; });
//
//            effect_object_view->c.make_binding(
//                    &effect_object_view->owned_lookup_box->rect.y,
//                    {&owned_lookup_box->rect.y, &owned_lookup_box->rect.height}, [&]() {
//                        effect_object_view->owned_lookup_box->rect.y =
//                                owned_lookup_box->rect.y + owned_lookup_box->rect.height;
//                    });
//            update_all_tracked();
//        } else {
//            delete effect_lookup_box;
//            delete effect_object_view;
//            effect_object_view = nullptr;
//
//            effect_lookup_box = new LookupBox;
//            update_listenable(&effect_lookup_box->prompt, string("effect"));
//
//            effect_lookup_box->c.make_binding(&effect_lookup_box->rect.x, {&owned_lookup_box->rect.x}, [&]() {
//                effect_lookup_box->rect.x = owned_lookup_box->rect.x;
//            });
//
//            effect_lookup_box->c.make_binding(&effect_lookup_box->rect.y,
//                                              {&owned_lookup_box->rect.y, &owned_lookup_box->rect.height}, [&]() {
//                        effect_lookup_box->rect.y = owned_lookup_box->rect.y + owned_lookup_box->rect.height;
//                    });
//            update_all_tracked();
//            effect_lookup_box->on_lookup = effect_on_lookup;
//        }
//    });
//    c.make_listener(&do_then->effect, effect_change_listener);
}

DoThenView::~DoThenView() {
    c.destroy_all();

    delete owned_lookup_box;

    delete next_lookup_box;
    delete next_object_view;

//    delete effect_lookup_box;
//    delete effect_object_view;
}
