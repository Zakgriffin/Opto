#pragma once

#ifndef OPTO_DECLARE_CELL_H
#define OPTO_DECLARE_CELL_H

#include "stds.h"
#include "object_view.h"

struct DeclareCell {
    string *name;
};

void declare_cell_create_sub_object_views(ObjectView *parent);


#endif //OPTO_DECLARE_CELL_H
