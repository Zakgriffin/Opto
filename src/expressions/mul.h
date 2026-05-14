#pragma once

#ifndef OPTO_MUL_H
#define OPTO_MUL_H

#include "object_view.h"

typedef struct {
    void *multiplicand;
    void *multiplier;
} Mul;

VIEW_DECLARATIONS(mul)

#endif //OPTO_MUL_H
