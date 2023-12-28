#ifndef OPTO_DO_THEN_H
#define OPTO_DO_THEN_H

#include <vector>
#include "do_then.h"
#include "lookup_box.h"

using namespace std;

struct DoThen {
    void *effect;
    DoThen *next;
};

struct DoThenView {
    // data
    DoThen *do_then;

    // visual
    LookupBox *owned_lookup_box;

//    ObjectView *next_object_view;
    function<void(string)> next_on_lookup;
    Fn *next_change_listener;

    DoThenView(DoThen *do_then_in, LookupBox* lookup_box_in);

    ~DoThenView();
};

#endif //OPTO_DO_THEN_H
