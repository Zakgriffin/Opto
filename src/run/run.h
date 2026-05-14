#pragma once

#ifndef OPTO_RUN_H
#define OPTO_RUN_H

#include "globals.h"
#include "control_flow.h"

typedef struct DoThen DoThen;
typedef struct Variable Variable;

typedef struct {
    void *start;
    DoThen *current;
    stack<function<void()>> scope_stack;
    unordered_map<Variable*, Variable*> vars;
} Run;

void* evaluate_expression(void* expression);

VIEW_DECLARATIONS(run)

#endif //OPTO_RUN_H
