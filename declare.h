#pragma once

#ifndef OPTO_DECLARE_H
#define OPTO_DECLARE_H

#include "stds.h"
#include "object_view.h"

struct Declare {
    string *name;
};

VIEW_DECLARATIONS(declare)

#endif //OPTO_DECLARE_H
