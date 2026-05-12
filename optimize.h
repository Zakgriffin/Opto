#pragma once

#ifndef OPTO_OPTIMIZE_H
#define OPTO_OPTIMIZE_H

#include "object_view.h"
#include "serialize.h"

#include "llvm/Transforms/Utils/Cloning.h"

typedef struct {
    void* source;
    void* passes;
    void* stages;
} Optimize;

VIEW_DECLARATIONS(optimize)

#endif //OPTO_OPTIMIZE_H
