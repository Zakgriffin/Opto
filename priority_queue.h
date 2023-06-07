#ifndef OPTO_PRIORITY_QUEUE_H
#define OPTO_PRIORITY_QUEUE_H

#include <vector>

using namespace std;

template<typename T, bool compare(T, T)>
class PriorityQueue {
public:
    vector<T> vec;
    void push(T item);

    T pop();

    bool is_empty();

    int size();
};

template <typename T, bool compare(T, T)>
void PriorityQueue<T, compare>::push(T item) {
    for(int i = 0; i < vec.size(); i++) {
        if(vec[i] == item) return; // no duplicates

        if(compare(vec[i], item)) {
            vec.insert(vec.begin(), item);
            return;
        }
    }
    vec.push_back(item);
}

template <typename T, bool compare(T, T)>
T PriorityQueue<T, compare>::pop() {
    T popped = vec[0];
    vec.erase(vec.begin());
    return popped;
}

template <typename T, bool compare(T, T)>
bool PriorityQueue<T, compare>::is_empty() {
    return vec.empty();
}

template <typename T, bool compare(T, T)>
int PriorityQueue<T, compare>::size() {
    return vec.size();
}

void test_priority_queue();

#endif //OPTO_PRIORITY_QUEUE_H
