#include "do_then.h"
#include "lookup_box.h"
#include "reactivity.h"
#include "unknown.h"
#include "types.h"
#include "edit_flow.h"

// DoThenView

DoThenView::DoThenView(DoThen *do_then_in) {
    do_then = do_then_in;
    lookup_box = new LookupBox;

    effect_view = nullptr;
    next_view = nullptr;

    create_binding(&bounding_box.width, {&lookup_box, &effect_view, &next_view}, [&]() {
        bounding_box.width = lookup_box->get_bounding_box()->width + effect_view->get_bounding_box()->width;
    });

    create_binding(&bounding_box.height, {&lookup_box, &effect_view, &next_view}, [&]() {
        bounding_box.height = lookup_box->get_bounding_box()->height + next_view->get_bounding_box()->height;
    });

    update_listenable(&lookup_box->text, string("do-then"));

    effect_datum_listener = new Fn([&]() {
        delete effect_view;
        if (do_then->effect == nullptr) {
            effect_view = new DoThenEffectAsNullView(this);
        } else {
            effect_view = new DoThenEffectAsAnyView(this);
        }
    });
    create_datum_listener_run_now(&do_then->effect, effect_datum_listener);
    next_datum_listener = new Fn([&]() {
        delete next_view;
        if (is_unknown(do_then->next)) {
            next_view = new DoThenNextAsUnknownView(this);
        } else if (do_then->next == nullptr) {
            next_view = new DoThenNextAsNullView(this);
        } else if (is_do_then(do_then->next)) {
            next_view = new DoThenNextAsDoThenView(this);
        } else {
            printf("Error: next_datum_listener unrecognized type\n");
            abort();
        }
    });
    create_datum_listener_run_now(&do_then->next, next_datum_listener);

    end_data_sync();
}

DoThenView::~DoThenView() {
    destroy_datum_listener(&do_then->effect, effect_datum_listener);
    destroy_datum_listener(&do_then->next, next_datum_listener);

    delete lookup_box;
    delete effect_view;
    delete next_view;
}

// DoThenNextView

DoThenNextAsNullView::DoThenNextAsNullView(DoThenView *parent) {
    lookup_box = new LookupBox;
    update_listenable(&lookup_box->prompt, string("next"));

    align_below(&parent->lookup_box->rect, &lookup_box->rect);

    lookup_box->on_lookup = [=](const string &s) {
        auto overflow_parameters = all_overflow_parameters[&parent->do_then->next];

        void *object = default_text_to_object(s, overflow_parameters);

        update_listenable(&parent->do_then->next, (DoThen *) object);
        end_data_sync();
    };
}

DoThenNextAsNullView::~DoThenNextAsNullView() {
    detach_aligned(&lookup_box->rect);
    delete lookup_box;
}

DoThenNextAsDoThenView::DoThenNextAsDoThenView(DoThenView *parent) {
    do_then_view = new DoThenView(parent->do_then->next);
    do_then_view->lookup_box->on_lookup = [&, parent](const string &s) {
        auto overflow_parameters = all_overflow_parameters[&parent->do_then->next];
        push_front(overflow_parameters, do_then_view->do_then->effect);
        push_front(overflow_parameters, do_then_view->do_then->next);

        void *object = default_text_to_object(s, overflow_parameters);

        update_listenable(&parent->do_then->next, (DoThen *) object);
        end_data_sync();
    };

    align_below(&parent->bounding_box, &do_then_view->bounding_box);
}

DoThenNextAsDoThenView::~DoThenNextAsDoThenView() {
    detach_aligned(&do_then_view->bounding_box);
    delete do_then_view;
}

// DoThenEffectView

DoThenEffectAsNullView::DoThenEffectAsNullView(DoThenView *parent) {
    lookup_box = new LookupBox;

    update_listenable(&lookup_box->prompt, string("effect"));
    align_right(&parent->bounding_box, &lookup_box->rect);
}

DoThenEffectAsNullView::~DoThenEffectAsNullView() {
    detach_aligned(&lookup_box->rect);
    delete lookup_box;
}

DoThenEffectAsAnyView::DoThenEffectAsAnyView(DoThenView *parent) {
    // TODO
}

DoThenEffectAsAnyView::~DoThenEffectAsAnyView() {
    // TODO
}

DoThenNextAsListView::DoThenNextAsListView(DoThenView *parent) {
    auto items = (vector<void *> *) parent->do_then->next;
    list_view = new ListView(items);

    align_below(&parent->lookup_box->rect, &list_view->rect);
}

DoThenNextAsListView::~DoThenNextAsListView() {
    detach_aligned(&list_view->rect);
    delete list_view;
}