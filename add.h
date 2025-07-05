#pragma once

#ifndef OPTO_ADD_H
#define OPTO_ADD_H

#include "stds.h"
#include "object_view.h"
#include "integer.h"

typedef struct Integer Integer;
typedef struct {
    Integer *augend;
    Integer *addend;
} Add;

VIEW_DECLARATIONS(add)

#endif //OPTO_ADD_H
