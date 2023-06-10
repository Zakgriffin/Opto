#include "unknown.h"

#include "lookup_box.h"
#include "globals.h"

UnknownView::UnknownView() {
    primary_lookup_box = new LookupBox("unknown");
    visuals.insert(primary_lookup_box);
}