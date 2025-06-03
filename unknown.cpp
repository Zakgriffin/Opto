#include "unknown.h"

void* unknown_at(Unknown* unknown, int i) {
    cout  << unknown->items.size() << endl;
    return nullptr;
    cout  << i << endl;
    if(i >= unknown->items.size()) return nullptr;
    cout << "WAHHH";
    return unknown->items.at(i);
}