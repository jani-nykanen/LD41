// LD41
// Splash (header)
// (c) 2018 Jani Nykänen

#ifndef __SPLASH__
#define __SPLASH__

#include "../include/renderer.h"
#include "../include/system.h"
#include "../include/std.h"

// Splash type
typedef struct {

    VEC2 pos;
    SPRITE spr;
    bool exist;
}
SPLASH;

// Init
void init_splash(ASSET_PACK* ass);

// Create a splash
SPLASH create_splash(VEC2 pos);

// Update splash
void splash_update(SPLASH* s, float tm);

// Draw a splash
void splash_draw(SPLASH* s);

#endif // __SPLASH__
