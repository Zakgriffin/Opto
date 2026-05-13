#pragma once

#ifndef OPTO_STRING_H
#define OPTO_STRING_H

#include "object_view.h"

typedef struct ObjectView ObjectView;

void string_create_sub_object_views(ObjectView *string_view);

void string_destroy_sub_object_views(ObjectView *string_view);

VIEW_DECLARATIONS(string)

#endif //OPTO_STRING_H
