#include "unknown.h"
#include "globals.h"
#include "reactivity.h"

#include "do_then.h"
#include "types.h"

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

void* e(string text) {
    void* object;
    if (text == "do-then") {
        object = new DoThen{
                .effect = nullptr,
                .next = nullptr,
        };
        make_do_then(object);
    }
}