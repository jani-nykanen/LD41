// LD41
// Global status (source)
// (c) 2018 Jani Nykänen

#include "status.h"

// Status
static STATUS st;


// Reset the global status
void reset_status() {

    int i = 0;
    for(; i < 4; ++ i) {

        st.items[i] = false;
    }

    st.blueCount = 0;
    st.redCount = 0;
    st.ammo = 5;

    st.time = 30.0f * 60.0f;
}


// Update status
void status_update(float tm) {

    st.time -= 1.0f * tm;
    if(st.time <= 0.0f) {

        st.time = 0.0f;
    }
}


// Get status
STATUS* get_status() {

    return &st;
}


// Get time string
void status_get_time_string(char* str) {

    int t = (int) (st.time/60.0f);
    int sec = t % 60;
    int min = t / 60;

    if(sec < 10)
        snprintf(str,16, "%d:0%d",min, sec);
    else
        snprintf(str,16, "%d:%d",min, sec);
    
}
