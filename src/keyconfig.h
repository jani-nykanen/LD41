// LD41
// Key configuration (header)
// (c) 2018 Jani Nykänen

#ifndef __KEYCONFIG__
#define __KEYCONFIG__

// Action count
#define ACTION_COUNT 2

// Key enumerations
enum {

    ACTION_PAUSE = 0,
    ACTION_ESCAPE = 1,
};


// Initialize default key config
void key_init_default();

// Get action state
int get_action_state(int action);

#endif // __KEYCONFIG__
