#pragma once

#include "stds.h"

struct Unknown {
    vector<void*> items;
};

void* unknown_at(Unknown* unknown, int i);

