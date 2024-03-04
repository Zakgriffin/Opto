#ifndef OPTO_EDIT_FLOW_H
#define OPTO_EDIT_FLOW_H

#include "globals.h"

extern unordered_map<void*, vector<void*>> all_overflow_parameters;

void push_front(vector<void *> vec, void *item);

void *default_text_to_object(const string &text, const vector<void *>& parameters);

#endif //OPTO_EDIT_FLOW_H
