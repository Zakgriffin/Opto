#pragma once

#include "globals.h"

struct DoThen {
    void *effect;
    void *next;
};

VIEW_DECLARATIONS(do_then)

string do_then_as_str(DoThen *do_then);

