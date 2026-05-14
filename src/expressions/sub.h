#pragma once

#ifndef OPTO_SUB_H
#define OPTO_SUB_H

#include "globals.h"

typedef struct {
    void *minuend;
    void *subtrahend;
} Sub;

VIEW_DECLARATIONS(sub)

#endif //OPTO_SUB_H
