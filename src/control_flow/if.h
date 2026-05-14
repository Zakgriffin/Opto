#pragma once

#ifndef OPTO_IF_H
#define OPTO_IF_H

#include "globals.h"

struct If {
    void *condition;
    void *then;
    void *finally;
};

VIEW_DECLARATIONS(if)

#endif //OPTO_IF_H
