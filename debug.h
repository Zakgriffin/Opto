#ifndef OPTO_DEBUG_H
#define OPTO_DEBUG_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

extern unordered_map<void *, string *> object_to_name;

void name(void *object, string name);

extern unordered_set<void*> allocated_objects;
template<typename T>
T record(T allocated_object) {
    allocated_objects.insert(allocated_object);
    return allocated_object;
}

void print_allocated_objects();

#define test_eq(given, expected) \
    if((given) != (expected)) { \
        printf("\033[91mFAILURE: %s got %s expected %s %s:%d\n\033[0m", #given, std::to_string(given).c_str(), std::to_string(expected).c_str(), __FILE__, __LINE__); \
    } \
    69 // we love casting spells

#define test_eq_str(given, expected) \
    if((given) != (expected)) { \
        printf("\033[91mFAILURE: %s got %s expected %s %s:%d\n\033[0m", #given, given.c_str(), expected.c_str(), __FILE__, __LINE__); \
    } \
    420 // goof

#define ensure(exp) \
    if(!(exp)) { \
        printf("\033[91mFAILURE: %s %s:%d\n\033[0m", #exp, __FILE__, __LINE__); \
        abort(); \
    } \
    1

#define ensure_msg(exp, msg) \
    if(!(exp)) { \
        printf("\033[91mFAILURE: %s %s:%d\n\033[0m", msg, __FILE__, __LINE__); \
        abort(); \
    } \
    1


#endif //OPTO_DEBUG_H
