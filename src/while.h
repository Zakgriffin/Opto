#pragma once

#ifndef OPTO_WHILE_H
#define OPTO_WHILE_H

#include "stds.h"
#include "object_view.h"

typedef struct ObjectView ObjectView;

struct While {
    void *condition;
    void *then;
    void *finally;
};

VIEW_DECLARATIONS(while)

#endif //OPTO_WHILE_H
