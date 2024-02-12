#include "globals.h"
#include "list.h"
#include "reactivity.h"
#include "do_then.h"
#include "unknown.h"
#include "types.h"

ListView::ListView(vector<void *> *list_in) {
    list = list_in;
    item_views = new vector<HasBoundingBox *>();

    begin_data_sync();

    // TODO should really "listen" on a per child basis
    list_datum_listener = new Fn([&]() {
        delete item_views;
        item_views = new vector<HasBoundingBox *>;

        for (auto item: *list) {
            HasBoundingBox *bounding_box_view;

            if (is_do_then(item)) {
                bounding_box_view = new DoThenView((DoThen *) item);
            } else if (is_unknown(item)) {
                bounding_box_view = new UnknownView((Unknown *) item);
            } else {
                printf("Could not default view for this. Type not recognized\n");
                abort();
            }

            item_views->push_back(bounding_box_view);
        }

        begin_data_sync();
        for (int i = 0; i < list->size(); i++) {
            auto bound = item_views->at(i)->get_bounding_box();
            auto last_bound = i == 0 ? &rect : item_views->at(i - 1)->get_bounding_box();

            create_binding(&bound->x, {&last_bound->x, &last_bound->width}, [=]() {
                bound->x = last_bound->x + last_bound->width;
            });
            create_binding(&bound->y, {&last_bound->y}, [=]() {
                bound->y = last_bound->y;
            });
        }
        end_data_sync();
    });
    create_datum_listener_run_now(&list, list_datum_listener);

    end_data_sync();
}