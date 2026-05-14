#pragma once

#ifndef OPTO_GREATER_THAN_H
#define OPTO_GREATER_THAN_H

#include "globals.h"

typedef struct {
    void *left;
    void *right;
} GreaterThan;

VIEW_DECLARATIONS(greater_than)

#endif //OPTO_GREATER_THAN_H
