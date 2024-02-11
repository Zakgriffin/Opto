#ifndef OPTO_TYPES_H
#define OPTO_TYPES_H

bool is_list(void* maybe_list);
void make_list(void* maybe_list);

bool is_do_then(void* maybe_do_then);
void make_do_then(void* maybe_do_then);

bool is_unknown(void* maybe_unknown);
void make_unknown(void* maybe_unknown);

#endif //OPTO_TYPES_H
