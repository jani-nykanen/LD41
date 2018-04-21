// LD41
// Key configuration (header)
// (c) 2018 Jani Nykänen

#ifndef __KEYCONFIG__
#define __KEYCONFIG__

// Action count
#define ACTION_COUNT 10

// Key enumerations
enum {

    ACTION_RIGHT = 0,
    ACTION_UP = 1,
    ACTION_LEFT = 2,
    ACTION_DOWN = 3,

    ACTION_RIGHT2 = 4,
    ACTION_UP2 = 5,
    ACTION_LEFT2 = 6,
    ACTION_DOWN2 = 7,

    ACTION_PAUSE = 8,
    ACTION_ESCAPE = 9,
};


// Initialize default key config
void key_init_default();

// Get action state
int get_action_state(int action);

#endif // __KEYCONFIG__
