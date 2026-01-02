#pragma once

#ifndef OPTO_LOOP_H
#define OPTO_LOOP_H

#include "stds.h"
#include "object_view.h"

struct Loop {
    void *body;
};

VIEW_DECLARATIONS(loop)

#endif //OPTO_LOOP_H
