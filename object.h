#pragma once

#ifndef OPTO_OBJECT_H
#define OPTO_OBJECT_H

#include "stds.h"

typedef enum {
    UNKNOWN,
    DO_THEN
} ObjectType;

void create_object(ObjectType type, Vector2 position);

#endif //OPTO_OBJECT_H
