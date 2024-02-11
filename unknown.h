#ifndef OPTO_UNKNOWN_H
#define OPTO_UNKNOWN_H

#include "globals.h"
#include "has_bounding_box.h"
#include "lookup_box.h"

struct Unknown {
    string s;
};


class UnknownView : public HasBoundingBox {
public:
    Unknown *unknown;
    LookupBox *lookup_box;

    explicit UnknownView(Unknown *unknown_in);

    Rectangle *get_bounding_box() override {
        return &lookup_box->rect;
    }

    ~UnknownView();
};

#endif //OPTO_UNKNOWN_H
