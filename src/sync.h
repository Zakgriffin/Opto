#pragma once

#include "globals.h"

extern unordered_map<void*, int> reference_counts;

void* counted_ref(void* object);
void lift_reference_temp(void *object);

template<typename T>
T* counted_ref(T* object) {
    reference_counts[object] = 0;
    lift_reference_temp(object);

    return object;
}