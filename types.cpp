#include "types.h"
#include "globals.h"

unordered_set<void *> all_lists;

bool is_list(void *maybe_list) {
    return all_lists.contains(maybe_list);
}

void make_list(void *maybe_list) {
    all_lists.insert(maybe_list);
}

unordered_set<void *> all_do_thens;

bool is_do_then(void *maybe_do_then) {
    return all_do_thens.contains(maybe_do_then);
}

void make_do_then(void *maybe_do_then) {
    all_do_thens.insert(maybe_do_then);
}

unordered_set<void *> all_unknowns;

bool is_unknown(void *maybe_unknown) {
    return all_unknowns.contains(maybe_unknown);
}

void make_unknown(void *maybe_unknown) {
    all_unknowns.insert(maybe_unknown);
}