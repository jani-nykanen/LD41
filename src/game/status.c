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
