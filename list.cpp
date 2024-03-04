#include "globals.h"
#include "reactivity.h"
#include "do_then.h"
#include "unknown.h"
#include "types.h"
#include "list.h"

// ListView

ListView::ListView(vector<void *> *list_in) {
    list = list_in;
    item_views = new vector<ListItemView *>();

    // TODO should really "listen" on a per child basis
    list_datum_listener = new Fn([&]() {
        for(auto item_view : *item_views) delete item_view;
        delete item_views;
        item_views = new vector<ListItemView *>;

        for (int i = 0; i < list->size(); i++) {
            auto item = list->at(i);
            ListItemView *list_item_view;

            if (is_do_then(item)) {
                list_item_view = new ListItemAsDoThenView(this, i);
            } else if (is_unknown(item)) {
                list_item_view = new ListItemAsUnknownView(this, i);
            } else {
                printf("Could not default view for this. Type not recognized\n");
                abort();
            }

            item_views->push_back(list_item_view);
        }
    });
    create_datum_listener_run_now(&list, list_datum_listener);

    end_data_sync();
}

ListView::~ListView() {
    destroy_datum_listener(&list, list_datum_listener);
}

// ListItemView

ListItemView::~ListItemView() = default;

ListItemAsUnknownView::ListItemAsUnknownView(ListView *parent, int i) {
    unknown_view = new UnknownView((Unknown *) (parent->list->at(i)));

    unknown_view->lookup_box->on_lookup = [=](const string &s) {
        if (s == "do-then") {
            auto do_then = new DoThen{
                    .effect = nullptr,
                    .next = nullptr,
            };
            make_do_then(do_then);

            parent->list->at(i) = do_then;
            update_listenable(&parent->list, parent->list);
            end_data_sync();
        }
    };
}

ListItemAsUnknownView::~ListItemAsUnknownView() {
    delete unknown_view;
}

ListItemAsDoThenView::ListItemAsDoThenView(ListView *parent, int i) {
    do_then_view = new DoThenView((DoThen *) (parent->list->at(i)));

    do_then_view->lookup_box->on_lookup = [&, parent](const string &s) {
        auto parameters = new vector<void *>;
        make_list(parameters);

        auto unknown = new Unknown{.s=do_then_view->lookup_box->text};
        make_unknown(unknown);
        parameters->push_back(unknown);
        if (do_then_view->do_then->effect != nullptr) {
            parameters->push_back(do_then_view->do_then->effect);
        }
        if (do_then_view->do_then->next != nullptr) {
            parameters->push_back(do_then_view->do_then->next);
        }

        parent->list[i].at(i) = unknown;
        update_listenable(&parent->list, parent->list);
        end_data_sync();
    };
}

ListItemAsDoThenView::~ListItemAsDoThenView() {
    auto bound = item_views->at(i)->get_bounding_box();
    auto last_bound = i == 0 ? &rect : item_views->at(i - 1)->get_bounding_box();

    align_right(last_bound, bound);
    end_data_sync();

    delete do_then_view;
}