#pragma once

#ifndef OPTO_COMPILE_H
#define OPTO_COMPILE_H

#include "globals.h"
#include "serialize.h"

typedef struct {
    void* source;
    void* passes;
    void* stages;
} Compile;

VIEW_DECLARATIONS(compile)

#endif //OPTO_COMPILE_H
