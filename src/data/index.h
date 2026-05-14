#pragma once

#ifndef OPTO_INDEX_H
#define OPTO_INDEX_H

#include "globals.h"

typedef struct {
    void *array;
    void *at;
} Index;

VIEW_DECLARATIONS(index)

#endif //OPTO_INDEX_H
