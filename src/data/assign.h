#pragma once

#ifndef OPTO_ASSIGN_H
#define OPTO_ASSIGN_H

#include "globals.h"

typedef struct {
    void *grantee;
    void *grantor;
} Assign;

VIEW_DECLARATIONS(assign)

#endif //OPTO_ASSIGN_H
