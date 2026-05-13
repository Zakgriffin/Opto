#include "unknown.h"

void* unknown_at(Unknown* unknown, int i) {
    return nullptr;
    if(i >= unknown->items.size()) return nullptr;
    cout << "WAHHH";
    return unknown->items.at(i);
}