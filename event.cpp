#include "event.h"

void signal_update(Signal *signal) {
    for (auto o: signal->on_change) (*o)();
}

Listener create_listener(const vector<Signal*>& signals, function<void(void)> *f) {
    for (auto signal: signals) {
        signal->on_change.insert(f);
    }
    (*f)();
    return Listener{.f = f, .signals=signals};
}

void destroy_listener(const Listener& listener) {
    for (auto signal: listener.signals) {
        signal->on_change.erase(listener.f);
    }
    delete listener.f;
}