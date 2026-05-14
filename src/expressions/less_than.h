#pragma once

#ifndef OPTO_LESS_THAN_H
#define OPTO_LESS_THAN_H

#include "globals.h"

typedef struct {
    void *left;
    void *right;
} LessThan;

VIEW_DECLARATIONS(less_than)

#endif //OPTO_LESS_THAN_H
