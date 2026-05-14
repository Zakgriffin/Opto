#pragma once

#include "globals.h"

struct While {
    void *condition;
    void *then;
    void *finally;
};

VIEW_DECLARATIONS(while)

