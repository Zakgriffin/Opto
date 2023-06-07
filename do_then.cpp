#include "do_then.h"

#include <vector>
#include "lookup_box.h"
#include "globals.h"
#include "constraint.h"

using namespace std;

DoThenView::DoThenView(LookupBox *owned_lookup_box) {
    this->owned_lookup_box = owned_lookup_box;

    next_lookup_box = new LookupBox("next");
    visuals.insert(next_lookup_box);

    create_binding(
            &next_lookup_box->rect.x,
            {&owned_lookup_box->rect.x, &owned_lookup_box->rect.width},
            [&]() { next_lookup_box->rect.x = owned_lookup_box->rect.x + owned_lookup_box->rect.width; }
    );

    create_binding(
            &next_lookup_box->rect.y,
            {&owned_lookup_box->rect.y},
            [&]() { next_lookup_box->rect.y = owned_lookup_box->rect.y; }
    );
}

void detach_from_owned_lookup_box() {

}

void DoThenView::destroy_view() {
    destroy_binding(&next_lookup_box->rect.x);
    destroy_binding(&next_lookup_box->rect.y);

    visuals.erase(visuals.find(next_lookup_box));
    delete next_lookup_box;
}
