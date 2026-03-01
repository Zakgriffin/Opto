#pragma once

#ifndef OPTO_CALL_H
#define OPTO_CALL_H

#include "object_view.h"

typedef struct {
    void *procedure;
    void *param;
} Call;

VIEW_DECLARATIONS(call)

#endif //OPTO_CALL_H
