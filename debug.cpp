#include "debug.h"

#include <utility>

using namespace std;

unordered_map<void *, string *> object_to_name;
unordered_set<void *> allocated_objects;

void name(void *object, string name) {
    object_to_name[object] = new string(std::move(name));
}

void print_allocated_objects() {
    printf("------\n");
    for(auto allocated_object : allocated_objects) {
        if(object_to_name.contains(allocated_object)) {
            printf("%s\n", object_to_name[allocated_object]->c_str());
        } else {
            printf("%p\n", allocated_object);
        }
    }
    printf("------\n");
}