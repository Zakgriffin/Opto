#ifndef OPTO_EVENT_H
#define OPTO_EVENT_H

#include "stds.h"
#include "globals.h"

struct SequencedEvent {
    Event *event;
    SequencedEvent *prev;
    SequencedEvent *next;
};

void ugh_test();

void subsume_lifetime_under_lifetime(void *subsumed, void *dominant);
void assign_datum(int *datum, int value);

#endif //OPTO_EVENT_H
