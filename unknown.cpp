#include "unknown.h"
#include "globals.h"
#include "reactivity.h"

#include "do_then.h"
#include "types.h"

UnknownView::UnknownView(Unknown *unknown_in) {
    unknown = unknown_in;
    lookup_box = new LookupBox;

    update_listenable(&lookup_box->text, unknown->s);
    end_data_sync();
}

UnknownView::~UnknownView() {
    delete lookup_box;
}