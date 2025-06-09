#pragma once

#ifndef OPTO_ADD_H
#define OPTO_ADD_H

#include "stds.h"
#include "object_view.h"
#include "integer.h"

typedef struct Integer Integer;
typedef struct {
    Integer *augend;
    Integer *addend;
} Add;

typedef struct ObjectView ObjectView;
typedef struct ObjectViewBuilder ObjectViewBuilder;

extern ObjectViewBuilder add_object_view_builder;

void* add_create_simple();

void add_create_sub_object_views(ObjectView *add_view);

void add_destroy_sub_object_views(ObjectView *add_view);

#endif //OPTO_ADD_H
