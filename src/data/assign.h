#pragma once

#ifndef OPTO_ASSIGN_H
#define OPTO_ASSIGN_H

#include "stds.h"
#include "object_view.h"

typedef struct {
    void *grantee;
    void *grantor;
} Assign;

VIEW_DECLARATIONS(assign)

#endif //OPTO_ASSIGN_H
