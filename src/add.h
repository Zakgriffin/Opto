#pragma once

#ifndef OPTO_ADD_H
#define OPTO_ADD_H

#include "object_view.h"

typedef struct {
    void *augend;
    void *addend;
} Add;

VIEW_DECLARATIONS(add)

#endif //OPTO_ADD_H
