#pragma once

#include "globals.h"

typedef struct {
    void* source;
    void* passes;
    void* stages;
} Compile;

VIEW_DECLARATIONS(compile)
