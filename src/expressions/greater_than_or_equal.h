#pragma once

#ifndef OPTO_GREATER_THAN_OR_EQUAL_H
#define OPTO_GREATER_THAN_OR_EQUAL_H

#include "globals.h"

typedef struct {
    void *left;
    void *right;
} GreaterThanOrEqual;

VIEW_DECLARATIONS(greater_than_or_equal)

#endif //OPTO_GREATER_THAN_OR_EQUAL_H
