
#include "edit_flow.h"
#include "globals.h"
#include "do_then.h"
#include "types.h"

unordered_map<void *, vector<void *>> all_overflow_parameters;

void push_front(vector<void *> vec, void *item) {
    vec.insert(vec.begin(), item);
}

void *take_one(vector<void *> vec) {
    auto item = vec.front();
    vec.erase(vec.begin());
    return item;
}

void *take_one_or_fallback(vector<void *> vec, void *fallback) {
    if (vec.empty()) return fallback;
    return take_one(vec);
}

void *default_text_to_object(const string &text, const vector<void *> &parameters) {
    void *object;
    if (text == "null") {
        object = nullptr;
    } else if (text == "do-then") {
        object = new DoThen{
                .effect = take_one_or_fallback(parameters, nullptr),
                .next = (DoThen *) take_one_or_fallback(parameters, nullptr),
        };
        make_do_then(object);
    } else if (text == "list") {
        object = new vector<void *>;
        make_list(object);
        while (!parameters.empty()) ((vector<void *> *) object)->push_back(take_one(parameters));
    } else {
        object = new Unknown{text};
        make_unknown(object);
    }

    return object;
}