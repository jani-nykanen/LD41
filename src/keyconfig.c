// LD41
// Key configuration (source)
// (c) 2018 Jani Nykänen

#include "keyconfig.h"

#include "include/system.h"
#include "include/std.h"

#include <SDL2/SDL.h>


// Array of actions
static int actions[ACTION_COUNT];


// Initialize default key config
void key_init_default() {

    actions[ACTION_PAUSE] = (int)SDL_SCANCODE_RETURN;
    actions[ACTION_ESCAPE] = (int)SDL_SCANCODE_ESCAPE;
}


// Get action state
int get_action_state(int action) {

    if(action < 0 || action >= ACTION_COUNT)
        return STATE_UP;
        
    return input_get_key(actions[action]);
}
