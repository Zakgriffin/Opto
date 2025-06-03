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

void do_then_create_sub_object_views(ObjectView *do_then_view);

void do_then_destroy_sub_object_views(ObjectView *do_then_view);

void *do_then_from_unknown(Unknown *unknown);

Unknown *do_then_to_unknown(void *object);

#endif //OPTO_DO_THEN_H
