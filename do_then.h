#ifndef OPTO_DO_THEN_H
#define OPTO_DO_THEN_H

#include "stds.h"
#include "object_view.h"

struct DoThen {
    void* effect;
    DoThen *next;
};


#endif //OPTO_DO_THEN_H
