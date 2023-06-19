#ifndef OPTO_UNKNOWN_H
#define OPTO_UNKNOWN_H

#include "object_view.h"
#include "lookup_box.h"

class UnknownView : ObjectView {
public:
    LookupBox *primary_lookup_box;

    UnknownView();
    ~UnknownView();
};

#endif //OPTO_UNKNOWN_H
