#include "do_then.h"
#include "lookup_box.h"
#include "reactivity.h"
#include "unknown.h"
#include "types.h"

DoThenView::DoThenView(DoThen *do_then_in) {
    do_then = do_then_in;
    lookup_box = new LookupBox;

    effect_view = nullptr;
    next_view = nullptr;

    begin_data_sync();
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
        if (is_list(do_then->next)) {
            next_view = new DoThenNextAsListView(this);
        } else if (do_then->next == nullptr) {
            next_view = new DoThenNextAsNullView(this);
        } else if (is_do_then(do_then->next)) {
            next_view = new DoThenNextAsDoThenView(this);
        } else {
            printf("VERY BAD DONT HAPPEN!!!!\n");
            abort();
        }
    });
    create_datum_listener_run_now(&do_then->next, next_datum_listener);

    end_data_sync();
}

DoThenView::~DoThenView() {
    destroy_datum_listener(&do_then->effect, effect_datum_listener);
    destroy_datum_listener(&do_then->next, next_datum_listener);

    begin_data_sync();
    delete lookup_box;
    delete effect_view;
    delete next_view;
    end_data_sync();
}

void attach_next(Rectangle *parent_rect, Rectangle *next_rect) {
    create_binding(&next_rect->x, {&parent_rect->x}, [=]() {
        next_rect->x = parent_rect->x;
    });
    create_binding(&next_rect->y, {&parent_rect->y, &parent_rect->height}, [=]() {
        next_rect->y = parent_rect->y + parent_rect->height;
    });
}

void attach_effect(Rectangle *parent_rect, Rectangle *effect_rect) {
    create_binding(&effect_rect->x, {&parent_rect->x, &parent_rect->width}, [=]() {
        effect_rect->x = parent_rect->x + parent_rect->width;
    });
    create_binding(&effect_rect->y, {&parent_rect->y}, [=]() {
        effect_rect->y = parent_rect->y;
    });
}

DoThenNextAsNullView::DoThenNextAsNullView(DoThenView *parent) {
    lookup_box = new LookupBox;
    lookup_box->on_lookup = [=](const string &s) {
        if (s == "do-then") {
            begin_data_sync();
            auto do_then = new DoThen{
                    .effect = nullptr,
                    .next = nullptr,
            };
            make_do_then(do_then);

            update_listenable(&parent->do_then->next, do_then);
            end_data_sync();
        }
    };

    update_listenable(&lookup_box->prompt, string("next"));

    attach_next(&parent->lookup_box->rect, &lookup_box->rect);
}

DoThenNextAsNullView::~DoThenNextAsNullView() {
    destroy_binding(&lookup_box->rect.x);
    destroy_binding(&lookup_box->rect.y);

    delete lookup_box;

    visuals.erase(lookup_box);
}

DoThenNextAsDoThenView::DoThenNextAsDoThenView(DoThenView *parent) {
    do_then_view = new DoThenView(parent->do_then->next);
    do_then_view->lookup_box->on_lookup = [&, parent](const string &s) {
        begin_data_sync();

        auto parameters = new vector<void *>;
        make_list(parameters);

        auto unknown = new Unknown(do_then_view->lookup_box->text);
        make_unknown(unknown);
        parameters->push_back(unknown);
        if (do_then_view->do_then->effect != nullptr) {
            parameters->push_back(do_then_view->do_then->effect);
        }
        if (do_then_view->do_then->next != nullptr) {
            parameters->push_back(do_then_view->do_then->next);
        }

        update_listenable(&parent->do_then->next, (DoThen *) (void *) parameters);
        end_data_sync();
    };

    attach_next(&parent->lookup_box->rect, &do_then_view->lookup_box->rect);
}

DoThenNextAsDoThenView::~DoThenNextAsDoThenView() {
    destroy_binding(&do_then_view->lookup_box->rect.x);
    destroy_binding(&do_then_view->lookup_box->rect.y);

    delete do_then_view;
//    update_listenable(&do_then_view->lookup_box->rect.x, do_then_view->lookup_box->rect.x + 50);
//    update_listenable(&do_then_view->lookup_box->rect.y, do_then_view->lookup_box->rect.y + 50);
}

DoThenEffectAsNullView::DoThenEffectAsNullView(DoThenView *parent) {
    lookup_box = new LookupBox;

    update_listenable(&lookup_box->prompt, string("effect"));
    attach_effect(&parent->lookup_box->rect, &lookup_box->rect);
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

DoThenNextAsListView::DoThenNextAsListView(DoThenView *parent) {
    list_view = new ListView((vector<void *> *) (void *) parent->do_then->next);

    // TODO should be per child listening
    for(auto x : *list_view->item_views) {

    }

    attach_next(&parent->lookup_box->rect, &list_view->rect);
}

DoThenNextAsListView::~DoThenNextAsListView() = default;