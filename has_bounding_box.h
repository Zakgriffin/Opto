#ifndef OPTO_HAS_BOUNDING_BOX_H
#define OPTO_HAS_BOUNDING_BOX_H

#include "globals.h"

class HasBoundingBox {
public:
    virtual Rectangle *get_bounding_box() {
        printf("HasBoundingBox get_bounding_box not overridden");
        abort();
    }
};

void align_below(Rectangle *base, Rectangle *below);

void align_right(Rectangle *base, Rectangle *right);

void detach_aligned(Rectangle *rect);

#endif //OPTO_HAS_BOUNDING_BOX_H
