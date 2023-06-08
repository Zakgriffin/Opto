#include "do_then.h"

#include <vector>
#include "lookup_box.h"
#include "globals.h"
#include "constraint.h"

using namespace std;

DoThenView::DoThenView(LookupBox *owned_lookup_box_in) {
    owned_lookup_box = owned_lookup_box_in;

    effect_lookup_box = new LookupBox("effect");
    create_binding(
            &effect_lookup_box->rect.x,
            {&owned_lookup_box->rect.x, &owned_lookup_box->rect.width},
            [&]() { effect_lookup_box->rect.x = owned_lookup_box->rect.x + owned_lookup_box->rect.width; }
    );

    create_binding(
            &effect_lookup_box->rect.y,
            {&owned_lookup_box->rect.y},
            [&]() { effect_lookup_box->rect.y = owned_lookup_box->rect.y; }
    );
    visuals.insert(effect_lookup_box);

    next_lookup_box = new LookupBox("next");
    create_binding(
            &next_lookup_box->rect.x,
            {&owned_lookup_box->rect.x},
            [&]() { next_lookup_box->rect.x = owned_lookup_box->rect.x; }
    );

    create_binding(
            &next_lookup_box->rect.y,
            {&owned_lookup_box->rect.y, &owned_lookup_box->rect.height},
            [&]() { next_lookup_box->rect.y = owned_lookup_box->rect.y + owned_lookup_box->rect.height; }
    );
    visuals.insert(next_lookup_box);
}

void detach_from_owned_lookup_box() {

}

void DoThenView::destroy_view() {
    destroy_binding(&next_lookup_box->rect.x);
    destroy_binding(&next_lookup_box->rect.y);
    visuals.erase(visuals.find(next_lookup_box));
    delete next_lookup_box;

    destroy_binding(&effect_lookup_box->rect.x);
    destroy_binding(&effect_lookup_box->rect.y);
    visuals.erase(visuals.find(effect_lookup_box));
    delete effect_lookup_box;
}
