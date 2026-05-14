#ifndef OPTO_CODEGEN_H
#define OPTO_CODEGEN_H

#include "object.h"
#include "control_flow.h"

typedef struct {
    void *condition;
    void *jump;
} ConditionalJump;
VIEW_DECLARATIONS(conditional_jump)

typedef struct {
    void *jump;
} Jump;
VIEW_DECLARATIONS(jump)

void append_do_then(void*** current_handle, void* effect);

void traverse_over_scopes(void* current, function<void(DoThen*)> on_each, function<void(void*, ObjectType)> on_open, function<void(void*, ObjectType)> on_close);
void* removed_scope_flow(void* flow);
void traverse_over_flat(void* flow, function<void(DoThen*)> on_each);

#endif //OPTO_CODEGEN_H