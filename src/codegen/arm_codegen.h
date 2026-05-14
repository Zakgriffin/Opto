#pragma once

#include "globals.h"

// register

typedef struct {

} ArmRegister;
VIEW_DECLARATIONS(arm_register)

// instruction

typedef struct {
    void *reg_moved;
    int *immediate;
} ArmMoveStatus;
VIEW_DECLARATIONS(arm_move_status)

typedef struct {
    void *reg_stored;
    void *reg_address;
    int *offset;
} ArmStoreRegister;
VIEW_DECLARATIONS(arm_store_register)

typedef struct {
    void *reg_stored;
    void *reg_address;
    int *offset;
} ArmLoadRegister;
VIEW_DECLARATIONS(arm_load_register)

typedef struct {
    void *reg_augend;
    void *reg_addend;
    void *immediate;
} ArmAdd;
VIEW_DECLARATIONS(arm_add)

typedef struct {
    void *reg_minuend;
    void *reg_subtrahend;
    void *immediate;
} ArmSubtract;
VIEW_DECLARATIONS(arm_subtract)

typedef struct {
    void *reg_compared;
    int* immediate;
} ArmCompare;
VIEW_DECLARATIONS(arm_compare)

typedef struct {
    void *label;
} ArmBranch;
VIEW_DECLARATIONS(arm_branch)

typedef struct {
    void *label;
} ArmBranchLessThanOrEqual;
VIEW_DECLARATIONS(arm_branch_less_than_or_equal)

void* stackify_variables(void* flow);
void* expand_expressions(void* flow);
