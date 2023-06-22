#ifndef OPTO_DO_THEN_H
#define OPTO_DO_THEN_H

#include <vector>
#include "object_view.h"
#include "do_then.h"
#include "lookup_box.h"

using namespace std;

struct DoThen {
    void *effect;
    DoThen *next;
};

struct DoThenView : ObjectView {
    // data
    DoThen *do_then;

    // visual
    LookupBox *owned_lookup_box;
    LookupBox *effect_lookup_box;
    LookupBox *next_lookup_box;

    ObjectView *effect_object_view;
    DoThenView *next_object_view;

    function<void(string)> next_on_lookup;
    function<void(string)> effect_on_lookup;
    F *next_change_listener;
    F *effect_change_listener;

    CleanupContext c;

    DoThenView(DoThen *do_then_in, LookupBox* lookup_box_in);

    ~DoThenView();
};

#endif //OPTO_DO_THEN_H
