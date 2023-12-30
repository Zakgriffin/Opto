#include "do_then.h"

#include <vector>
#include "lookup_box.h"
#include "reactivity.h"

using namespace std;

DoThenView::DoThenView(DoThen *do_then_in) {
    do_then = do_then_in;
    lookup_box = new LookupBox;

    effect_view = nullptr;
    next_view = nullptr;

    begin_data_sync();
    update_listenable(&lookup_box->text, string("do-then"));

    create_datum_listener_run_now(&do_then->effect, new Fn([&]() {
        delete effect_view;
        if (do_then->effect == nullptr) {
            effect_view = new DoThenEffectAsNullView(this);
        } else {
            effect_view = new DoThenEffectAsAnyView(this);
        }
    }));
    create_datum_listener_run_now(&do_then->next, new Fn([&]() {
        delete next_view;
        if (do_then->next == nullptr) {
            next_view = new DoThenNextAsNullView(this);
        } else {
            next_view = new DoThenNextAsDoThenView(this);
        }
    }));

    end_data_sync();
}

DoThenView::~DoThenView() {
    delete lookup_box;
    delete effect_view;
    delete next_view;
}

void attach_next(DoThenView *parent, LookupBox *next_lookup_box) {
    create_binding(&next_lookup_box->rect.x, {&parent->lookup_box->rect.x}, [=]() {
        next_lookup_box->rect.x = parent->lookup_box->rect.x;
    });
    create_binding(&next_lookup_box->rect.y, {&parent->lookup_box->rect.y, &parent->lookup_box->rect.height}, [=]() {
        next_lookup_box->rect.y = parent->lookup_box->rect.y + parent->lookup_box->rect.height;
    });
}

void attach_effect(DoThenView *parent, LookupBox *effect_lookup_box) {
    create_binding(&effect_lookup_box->rect.x, {&parent->lookup_box->rect.x, &parent->lookup_box->rect.width}, [=]() {
        effect_lookup_box->rect.x = parent->lookup_box->rect.x + parent->lookup_box->rect.width;
    });
    create_binding(&effect_lookup_box->rect.y, {&parent->lookup_box->rect.y}, [=]() {
        effect_lookup_box->rect.y = parent->lookup_box->rect.y;
    });
}

DoThenNextAsNullView::DoThenNextAsNullView(DoThenView *parent) {
    lookup_box = new LookupBox;
    lookup_box->on_lookup = [=](const string &s) {
        if (s == "do-then") {
            begin_data_sync();
            update_listenable(&parent->do_then->next, new DoThen{
                    .effect = nullptr,
                    .next = nullptr,
            });
            end_data_sync();
        }
    };

    update_listenable(&lookup_box->prompt, string("next"));

    attach_next(parent, lookup_box);
}

DoThenNextAsNullView::~DoThenNextAsNullView() {
    destroy_binding(&lookup_box->rect.x);
    destroy_binding(&lookup_box->rect.y);

    delete lookup_box;

    visuals.erase(lookup_box);
}

DoThenNextAsDoThenView::DoThenNextAsDoThenView(DoThenView *parent) {
    do_then_view = new DoThenView(parent->do_then->next);
    do_then_view->lookup_box->on_lookup = [=](const string &s) {
//        if (s != "do-then") {
        begin_data_sync();
        update_listenable(&parent->do_then->next, (DoThen *) nullptr);
        end_data_sync();
//        }
    };

    attach_next(parent, do_then_view->lookup_box);
}

DoThenNextAsDoThenView::~DoThenNextAsDoThenView() {
    destroy_binding(&do_then_view->lookup_box->rect.x);
    destroy_binding(&do_then_view->lookup_box->rect.y);

    update_listenable(&do_then_view->lookup_box->rect.x, do_then_view->lookup_box->rect.x + 50);
    update_listenable(&do_then_view->lookup_box->rect.y, do_then_view->lookup_box->rect.y + 50);
}

DoThenEffectAsNullView::DoThenEffectAsNullView(DoThenView *parent) {
    lookup_box = new LookupBox;

    update_listenable(&lookup_box->prompt, string("effect"));
    attach_effect(parent, lookup_box);
}

DoThenEffectAsNullView::~DoThenEffectAsNullView() {
    destroy_binding(&lookup_box->rect.x);
    destroy_binding(&lookup_box->rect.y);

    delete lookup_box;

    visuals.erase(lookup_box);
}

DoThenEffectAsAnyView::DoThenEffectAsAnyView(DoThenView *parent) {
}

DoThenEffectAsAnyView::~DoThenEffectAsAnyView() = default;

DoThenNextView::~DoThenNextView() = default;

DoThenEffectView::~DoThenEffectView() = default;
