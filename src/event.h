#pragma once

#include "stds.h"

typedef struct {
    unordered_set<function<void(void)> *> on_change;
} Signal;

typedef struct {
    function<void(void)> *f;
    vector<Signal *> signals;
} Listener;

extern Signal draw_visuals;
extern Signal input_listeners;

void signal_update(Signal *signal);

Listener create_listener(const vector<Signal*>& signals, function<void(void)> *f);
Listener create_listener_lazy(const vector<Signal*>& signals, function<void(void)> *f);

void destroy_listener(const Listener& listener);
