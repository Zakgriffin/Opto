#pragma once

#ifndef OPTO_TYPE_H
#define OPTO_TYPE_H

#include "object_view.h"

void type_check(void *obj, map<void*, ObjectType> *env);

ObjectType env_type(void* obj, map<void*, ObjectType> *env);

bool is_control_flow(ObjectType type);

#endif //OPTO_TYPE_H
