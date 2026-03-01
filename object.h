#pragma once

#ifndef OPTO_OBJECT_H
#define OPTO_OBJECT_H

#include "stds.h"

typedef enum {
    NONE,
    DO_THEN,
    ADD,
    ASSIGN,
    RUN,
    INTEGER,
    STRING,
    DECLARE,
    IF,
    LOOP,
    WHILE,
    REPEAT,
    GREATER_THAN,
    PROCEDURE,
    CALL,

    CONDITIONAL_JUMP,
    JUMP,

    ARM_REGISTER,

    ARM_MOVE_STATUS,
    ARM_ADD,
    ARM_SUBTRACT,
    ARM_LOAD_REGISTER,
    ARM_STORE_REGISTER,
    ARM_COMPARE,
    ARM_BRANCH,
    ARM_BRANCH_LESS_THAN_OR_EQUAL,
} ObjectType;

void create_object(ObjectType type, Vector2 position);

#endif //OPTO_OBJECT_H
