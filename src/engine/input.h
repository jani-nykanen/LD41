// LD41
// Input (header)
// (c) 2018 Jani Nykänen

#ifndef __INPUT__
#define __INPUT__

#include <SDL2/SDL.h>

#include "vector.h"

// Input states
enum {

    STATE_UP = 0,
    STATE_DOWN = 1,
    STATE_PRESSED = 2,
    STATE_RELEASED = 3 
};

// initialize
void input_init();

// Key pressed
void input_key_down(int key);

// Key released
void input_key_up(int key);

// Mouse move
void input_mouse_move(int x, int y);

// Mouse button down
void input_mouse_down(int button);

// Mouse button up
void input_mouse_up(int button);

// Update input
void input_update();

// Get key state
Uint8 input_get_key(int key);

// Set view info
void input_set_view_info(POINT canvasPos, POINT canvasSize);

// Get mouse position
POINT input_get_cursor_pos();

// Get mouse button
int input_get_mouse_button(int button);

#endif // __INPUT__