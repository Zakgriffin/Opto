#pragma once

#include "globals.h"

struct If {
    void *condition;
    void *then;
    void *finally;
};

VIEW_DECLARATIONS(if)

