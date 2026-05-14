#pragma once

#ifndef OPTO_REPEAT_H
#define OPTO_REPEAT_H

#include "globals.h"

struct Repeat {
    void *then;
    void *finally;
    void *condition;
};

VIEW_DECLARATIONS(repeat)

#endif //OPTO_REPEAT_H
