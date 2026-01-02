#pragma once

#ifndef OPTO_REPEAT_H
#define OPTO_REPEAT_H

#include "stds.h"
#include "object_view.h"

typedef struct ObjectView ObjectView;

struct Repeat {
    void *then;
    DoThen *finally;
    void *condition;
};

VIEW_DECLARATIONS(repeat)

#endif //OPTO_REPEAT_H
