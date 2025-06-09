#pragma once

#ifndef OPTO_DO_THEN_H
#define OPTO_DO_THEN_H

#include "stds.h"
#include "object_view.h"

struct DoThen {
    void *effect;
    DoThen *next;
};

typedef struct ObjectView ObjectView;
typedef struct ObjectViewBuilder ObjectViewBuilder;

extern ObjectViewBuilder do_then_object_view_builder;

void do_then_create_sub_object_views(ObjectView *do_then_view);

void do_then_destroy_sub_object_views(ObjectView *do_then_view);

void *do_then_from_unknown(Unknown *unknown);

void* do_then_create_simple();

Unknown *do_then_to_unknown(void *object);

string do_then_as_str(DoThen *do_then);

#endif //OPTO_DO_THEN_H
