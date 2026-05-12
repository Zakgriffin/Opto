#pragma once

#ifndef OPTO_AND_H
#define OPTO_AND_H

#include "object_view.h"

typedef struct {
    void *conjunct_1;
    void *conjunct_2;
} And;

VIEW_DECLARATIONS(and)

#endif //OPTO_AND_H
