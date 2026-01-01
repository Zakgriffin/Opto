#pragma once

#ifndef OPTO_ADD_H
#define OPTO_ADD_H

#include "object_view.h"

typedef struct {
    int *augend;
    int *addend;
} Add;

VIEW_DECLARATIONS(add)

#endif //OPTO_ADD_H
