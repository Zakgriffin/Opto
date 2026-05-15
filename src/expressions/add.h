#pragma once

#include "globals.h"
#include "object_view_temp.h"

typedef struct {
    void *augend;
    void *addend;
} Add;

VIEW_DECLARATIONS(add)