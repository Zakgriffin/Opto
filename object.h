#pragma once

#ifndef OPTO_OBJECT_H
#define OPTO_OBJECT_H

#include "stds.h"

typedef enum {
    NONE,
    DO_THEN,
    ADD,
    SUB,
    MUL,
    AND,
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
    GREATER_THAN_OR_EQUAL,
    LESS_THAN,
    PROCEDURE,
    CALL,
    VECTOR,
    INDEX,
    OPTIMIZE,

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

    LLVM_PASS,

    LLVM_MODULE,
    LLVM_FUNCTION,
    LLVM_BASIC_BLOCK,

    LLVM_STORE,
    LLVM_LOAD,
    LLVM_ALLOCA,
    LLVM_ADD,
    LLVM_SUB,
    LLVM_MUL,
    LLVM_BR,
    LLVM_RET,
    LLVM_CALL,
    LLVM_ICMP,
    LLVM_ZEXT,
    LLVM_PHI,

    LLVM_VALUE
} ObjectType;

void create_object(ObjectType type, ray::Vector2 position);

#endif //OPTO_OBJECT_H
