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

#endif //OPTO_HAS_BOUNDING_BOX_H
