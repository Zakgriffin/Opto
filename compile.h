#ifndef OPTO_COMPILE_H
#define OPTO_COMPILE_H

#include "object_view.h"

void traverse_over_scopes(void* current, function<void(DoThen*)> on_each, function<void(void*, ObjectType)> on_open, function<void(void*, ObjectType)> on_close);
void* removed_scope_flow(void* flow);
void traverse_over_flat(void* flow, function<void(DoThen*)> on_each);

#endif //OPTO_COMPILE_H