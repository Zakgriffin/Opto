#pragma once

#ifndef OPTO_PROCEDURE_H
#define OPTO_PROCEDURE_H

#include "globals.h"

typedef struct {
    void *param;
    void *body;
} Procedure;

VIEW_DECLARATIONS(procedure)

#endif //OPTO_PROCEDURE_H
