#pragma once

#ifndef OPTO_INTEGER_H
#define OPTO_INTEGER_H

#include "object_view.h"

typedef struct ObjectView ObjectView;

void integer_create_sub_object_views(ObjectView *integer_view);

void integer_destroy_sub_object_views(ObjectView *integer_view);

VIEW_DECLARATIONS(integer)

#endif //OPTO_INTEGER_H
