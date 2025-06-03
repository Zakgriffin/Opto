#pragma once

#ifndef OPTO_STRING_H
#define OPTO_STRING_H

#include "stds.h"
#include "object_view.h"

struct String_ {
    string s;
};

typedef struct ObjectView ObjectView;

void string_create_sub_object_views(ObjectView *string_view);

void string_destroy_sub_object_views(ObjectView *string_view);

void *string_from_unknown(Unknown *unknown);

Unknown *string_to_unknown(void *object);

#endif //OPTO_STRING_H
