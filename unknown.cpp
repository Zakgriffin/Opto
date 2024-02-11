#include "unknown.h"
#include "globals.h"

UnknownView::UnknownView(Unknown *unknown_in) {
    unknown = unknown_in;
    lookup_box = new LookupBox;

    begin_data_sync();
    update_listenable(&lookup_box->text, unknown->s);
    end_data_sync();
}

UnknownView::~UnknownView() {
    delete lookup_box;
}