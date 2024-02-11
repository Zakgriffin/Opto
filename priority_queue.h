#ifndef OPTO_PRIORITY_QUEUE_H
#define OPTO_PRIORITY_QUEUE_H

#include <vector>

using namespace std;

template<typename T, bool compare(T, T)>
class PriorityQueue {
public:
    vector<T> vec;

    void push(T item) {
        for (int i = 0; i < vec.size(); i++) {
            if (vec[i] == item) return; // no duplicates

            if (compare(vec[i], item)) {
                vec.insert(vec.begin(), item);
                return;
            }
        }
        vec.push_back(item);
    }

    T pop() {
        T popped = vec[0];
        vec.erase(vec.begin());
        return popped;
    }

    bool is_empty() {
        return vec.empty();
    }

    int size() {
        return vec.size();
    }
};

#endif //OPTO_PRIORITY_QUEUE_H
