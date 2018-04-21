// LD41
// Input (header)
// (c) 2018 Jani Nykänen

#include "input.h"

#include "core.h"

#include "../include/renderer.h"
#include "../include/std.h"

// Key max
#define KEY_MAX 256

// Mouse button max
#define BUTTON_MAX 16

// Key states
static Uint8 keystates[KEY_MAX];
// Mouse buttons
static Uint8 mbuttons[BUTTON_MAX];

// Mouse position
static POINT mousePos;

// Canvas info
static POINT canvasPos;
static POINT canvasSize;


// initialize
void input_init() {

    int i = 0;
    // Set default values to the state arrays
    for(; i < KEY_MAX; ++ i) {
        
        keystates[i] = STATE_UP;
    }

    for(i = 0; i < BUTTON_MAX; ++ i) {
        
        mbuttons[i] = STATE_UP;
    }
}


// Key pressed
void input_key_down(int key) {

    // If the key is in range and not already down, make it be pressed
    if(key < 0 || key >= KEY_MAX || keystates[key] == STATE_DOWN) 
        return;
    
    keystates[key] = STATE_PRESSED;
}


// Key released
void input_key_up(int key) {

    // If the key is in range and not already down, make it be pressed
    if(key < 0 || key >= KEY_MAX || keystates[key] == STATE_UP) 
        return;
    
    keystates[key] = STATE_RELEASED;
}


// Mouse button pressed
void input_mouse_down(int button) {

    // If the button is in range and not already down, make it be pressed
    if(button < 0 || button >= BUTTON_MAX || mbuttons[button] == STATE_DOWN) 
        return;
    
    mbuttons[button] = STATE_PRESSED;
}


// Mouse button released
void input_mouse_up(int button) {

    // If the button is in range and not already down, make it be pressed
    if(button < 0 || button >= BUTTON_MAX || mbuttons[button] == STATE_UP) 
        return;
    
    mbuttons[button] = STATE_RELEASED;
}


// Mouse moved
void input_mouse_move(int x, int y) {

    mousePos = point(x,y);
}


// Update input
void input_update() {

    // Update key states
    int i = 0;
    for(; i < KEY_MAX; ++i)
    {
        if(keystates[i] == STATE_RELEASED)
            keystates[i] = STATE_UP;

        else if(keystates[i] == STATE_PRESSED)
            keystates[i] = STATE_DOWN;

        if(i < BUTTON_MAX) {

            if(mbuttons[i] == STATE_RELEASED)
                mbuttons[i] = STATE_UP;

            else if(mbuttons[i] == STATE_PRESSED)
                mbuttons[i] = STATE_DOWN;
        }
    }
}


// Get key state
Uint8 input_get_key(int key) {

    if(key < 0 || key >= KEY_MAX) return STATE_UP;

    return keystates[key];
}


// Set view info
void input_set_view_info(POINT pos, POINT size) {

    canvasPos = pos;
    canvasSize = size;
}


// Get mouse position
POINT input_get_cursor_pos() {

    POINT mpos = mousePos;
    FRAME* gframe = get_global_frame();
    POINT fsize = point(gframe->width, gframe->height);

    mpos.x -= canvasPos.x;
    mpos.y -= canvasPos.y;

    float sx = (float)fsize.x / (float)canvasSize.x;
    float sy = (float)fsize.y / (float)canvasSize.y;
     
    POINT ret;

    ret.x = (int)(sx* (float)mpos.x);
    ret.y = (int)(sy* (float)mpos.y);

    return ret;
}


// Get mouse button
int input_get_mouse_button(int button) {

    if(button < 0 || button >= BUTTON_MAX) return STATE_UP;

    return mbuttons[button];
}

