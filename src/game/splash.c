// LD41
// Splash (header)
// (c) 2018 Jani Nykänen

#include "splash.h"

// Bitmaps
static BITMAP* bmpSplash;

// Init
void init_splash(ASSET_PACK* ass) {

    // Get bitmaps
    bmpSplash = (BITMAP*) assets_get(ass, "splash");
}


// Create a splash
SPLASH create_splash(VEC2 pos) {
    
    SPLASH s;
    s.pos = pos;
    s.spr = create_sprite(16, 16);
    s.exist = true;

    return s;
}


// Update splash
void splash_update(SPLASH* s, float tm){

    if(!s->exist) return;

    spr_animate(&s->spr,0,0,6,3, tm);
    if(s->spr.frame == 6)
        s->exist = false;
}


// Draw a splash
void splash_draw(SPLASH* s) {

    if(!s->exist) return;

    spr_draw(&s->spr, bmpSplash, (int)s->pos.x-8, (int)s->pos.y-8, 0);
}
