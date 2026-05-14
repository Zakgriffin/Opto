#pragma once

#ifndef OPTO_RUN_H
#define OPTO_RUN_H

#include "globals.h"
#include "do_then.h"

typedef struct DoThen DoThen;
typedef struct Declare Declare;

typedef struct {
    void *start;
    DoThen *current;
    stack<function<void()>> scope_stack;
    unordered_map<Declare*, Declare*> vars;
} Run;

void* evaluate_expression(void* expression);

VIEW_DECLARATIONS(run)

#endif //OPTO_RUN_H
