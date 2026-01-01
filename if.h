#pragma once

#ifndef OPTO_IF_H
#define OPTO_IF_H

#include "stds.h"
#include "object_view.h"

typedef struct ObjectView ObjectView;

struct If {
    void *condition;
    DoThen *then;
    DoThen *finally;
};

VIEW_DECLARATIONS(if)

#endif //OPTO_IF_H
