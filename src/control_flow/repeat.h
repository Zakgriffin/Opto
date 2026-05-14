#pragma once

#include "globals.h"

struct Repeat {
    void *then;
    void *finally;
    void *condition;
};

VIEW_DECLARATIONS(repeat)

