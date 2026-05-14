#pragma once

#include "object.h"

void type_check(void *obj, unordered_map<void*, ObjectType> *env);

ObjectType env_type(void* obj, unordered_map<void*, ObjectType> *env);

bool is_control_flow(ObjectType type);

