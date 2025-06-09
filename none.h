#pragma once

#ifndef OPTO_NONE_H
#define OPTO_NONE_H

#include "stds.h"
#include "object_view.h"

typedef struct ObjectView ObjectView;
typedef struct ObjectViewBuilder ObjectViewBuilder;

extern ObjectViewBuilder none_object_view_builder;

void none_create_sub_object_views(ObjectView *none_view);

void none_destroy_sub_object_views(ObjectView *none_view);


void* none_create_simple();

#endif //OPTO_NONE_H
