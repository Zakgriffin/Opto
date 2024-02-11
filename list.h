#ifndef OPTO_LIST_H
#define OPTO_LIST_H

#include "has_bounding_box.h"

class ListView {
public:
    vector<void *> *list;

    vector<HasBoundingBox *> *list_views; // on_lookup should be assigned by creator right after construction

    Fn *list_datum_listener;

    Rectangle rect{}; // top and left bound externally, width and height bound internally

    explicit ListView(vector<void *> *);

    ~ListView();
};

#endif //OPTO_LIST_H
