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

    actions[ACTION_RIGHT] = (int)SDL_SCANCODE_RIGHT;
    actions[ACTION_UP] = (int)SDL_SCANCODE_UP;
    actions[ACTION_LEFT] = (int)SDL_SCANCODE_LEFT;
    actions[ACTION_DOWN] = (int)SDL_SCANCODE_DOWN;

    actions[ACTION_RIGHT2] = (int)SDL_SCANCODE_D;
    actions[ACTION_UP2] = (int)SDL_SCANCODE_W;
    actions[ACTION_LEFT2] = (int)SDL_SCANCODE_A;
    actions[ACTION_DOWN2] = (int)SDL_SCANCODE_S;

    actions[ACTION_PAUSE] = (int)SDL_SCANCODE_RETURN;
    actions[ACTION_ESCAPE] = (int)SDL_SCANCODE_ESCAPE;
}


// Get action state
int get_action_state(int action) {

    if(action < 0 || action >= ACTION_COUNT)
        return STATE_UP;
        
    return input_get_key(actions[action]);
}
