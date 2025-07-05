#pragma once

#ifndef OPTO_DO_THEN_H
#define OPTO_DO_THEN_H

#include "stds.h"
#include "object_view.h"

struct DoThen {
    void *effect;
    DoThen *next;
};

VIEW_DECLARATIONS(do_then)

string do_then_as_str(DoThen *do_then);

#endif //OPTO_DO_THEN_H
