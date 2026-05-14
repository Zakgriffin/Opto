#pragma once

#include "control_flow.h"
#include "globals.h"

typedef struct Variable Variable;

typedef struct {
    void *start;
    DoThen *current;
    stack<function<void()>> scope_stack;
    unordered_map<Variable*, Variable*> vars;
} Run;

void* evaluate_expression(void* expression);

VIEW_DECLARATIONS(run)
