#ifndef OPTO_DO_THEN_H
#define OPTO_DO_THEN_H

#include <vector>
#include "object_view.h"
#include "do_then.h"
#include "lookup_box.h"

using namespace std;

struct DoThen {
    DoThen* next;
    void* effect;
};

struct DoThenView : ObjectView {

    vector<float*> tracked_for_visuals; // internal visual constraints for all do_then

    // data
    DoThen* do_then;

    // visual
    LookupBox* owned_lookup_box;
    LookupBox* effect_lookup_box;
    LookupBox* next_lookup_box;

    DoThenView(LookupBox* owned_lookup_box);
    void destroy_view();
};

#endif //OPTO_DO_THEN_H
