#include "sync.h"
#include "object_view.h"

unordered_map<void*, int> reference_counts;

void lift_reference_temp(void *object) {
    if(!reference_counts.contains(object)) {
        cout << "not a counted ref" << endl;
        abort();
    }

    reference_counts[object] += 1;
}