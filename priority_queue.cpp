#include "priority_queue.h"

using namespace std;

bool comp_text(int left, int right) {
    return left < right;
}

void test_priority_queue() {
    PriorityQueue<int, comp_text> p;

    p.push(3);
    p.push(5);
    p.push(6);
    p.push(2);

    int size = p.size();

    while(!p.is_empty()) {
        int x = p.pop();
    }
}