#pragma once

#ifndef OPTO_UNKNOWN_H
#define OPTO_UNKNOWN_H

#include "stds.h"

struct Unknown {
    vector<void*> items;
};

void* unknown_at(Unknown* unknown, int i);

#endif //OPTO_UNKNOWN_H
