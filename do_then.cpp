#include "do_then.h"

#include <vector>
#include "lookup_box.h"
#include "reactivity.h"

using namespace std;

DoThenView::DoThenView(DoThen *do_then_in, LookupBox *owned_lookup_box_in) {
    do_then = do_then_in;
    owned_lookup_box = owned_lookup_box_in;

    // next
}

DoThenView::~DoThenView() {
    delete owned_lookup_box;
}
