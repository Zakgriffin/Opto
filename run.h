#pragma once

#ifndef OPTO_RUN_H
#define OPTO_RUN_H

#include "stds.h"
#include "object_view.h"
#include "do_then.h"

typedef struct DoThen DoThen;

typedef struct {
    void *start;
    void *current;
    stack<void*> scope_stack;
} Run;

VIEW_DECLARATIONS(run)

#endif //OPTO_RUN_H
