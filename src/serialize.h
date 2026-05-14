#pragma once

#ifndef OPTO_SERIALIZE_H
#define OPTO_SERIALIZE_H

#include "globals.h"

void serialize_ast(void* node, std::vector<char>& buffer);

#endif //OPTO_SERIALIZE_H
