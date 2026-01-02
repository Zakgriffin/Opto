#ifndef OPTO_ARM_COMPILE_H
#define OPTO_ARM_COMPILE_H

#include "object_view.h"

typedef struct {
    void *condition;
    void *jump;
} ConditionalJump;
VIEW_DECLARATIONS(conditional_jump)

typedef struct {
    void *jump;
} Jump;
VIEW_DECLARATIONS(jump)

// instruction

typedef struct {
    void *reg_augend;
    void *reg_addend;
} ArmAdd;

typedef struct {
    void *reg_moved;
    int *immediate;
} ArmMoveStatus;

typedef struct {
    void *reg_stored;
    void *reg_address;
    int *offset;
} ArmStoreRegister;

typedef struct {
    void *reg_stored;
    void *reg_address;
    int *offset;
} ArmLoadRegister;

typedef struct {
    void *reg_compared;
} ArmCompare;

typedef struct {
    void *label;
} ArmBranchLessThan;

typedef struct {
    void *label;
} ArmBranchLess;

void* removed_scope_flow(void* flow);

#endif //OPTO_ARM_COMPILE_H