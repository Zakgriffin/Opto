#pragma once

#include "stds.h"

typedef struct {
    long long last_click_millis;
    int click_streak;
} MultiClick;

void init_multi_click(MultiClick* m);
int check_clicked_n_times(MultiClick *m, int max_clicks);
