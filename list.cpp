#include "globals.h"
#include "list.h"
#include "reactivity.h"
#include "do_then.h"
#include "unknown.h"
#include "types.h"

using namespace std;

ListView::ListView(vector<void *> *list_in) {
    list = list_in;
    list_views = new vector<HasBoundingBox *>();

    begin_data_sync();

    // TODO should really "listen" on a per child basis
    list_datum_listener = new Fn([&]() {
        delete list_views;
        list_views = new vector<HasBoundingBox *>;

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

            list_views->push_back(bounding_box_view);
        }

        begin_data_sync();
        if (!list_views->empty()) {
            auto bound = list_views->at(0)->get_bounding_box();
            auto begin_bound = &rect;
            create_binding(&bound->x, {&begin_bound->x, &begin_bound->width}, [=]() {
                bound->x = begin_bound->x + begin_bound->width;
            });
            create_binding(&bound->y, {&begin_bound->y}, [=]() {
                bound->y = begin_bound->y;
            });
        }

        for (int i = 1; i < list->size(); i++) {
            auto bound = list_views->at(i)->get_bounding_box();
            auto last_bound = list_views->at(i - 1)->get_bounding_box();

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