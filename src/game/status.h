// LD41
// Global status (header)
// (c) 2018 Jani Nykänen

#ifndef __STATUS__
#define __STATUS__

#include "../include/std.h"

// Status type
typedef struct {

    bool items[4];
    int time;
    int ammo;

    int redCount;
    int blueCount;

}
STATUS;

// Reset the global status
void reset_status();

// Update status
void status_update(float tm);

// Get status
STATUS* get_status();

// Get time string
void status_get_time_string(char* str);

#endif // __STATUS__
