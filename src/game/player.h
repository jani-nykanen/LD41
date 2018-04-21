// LD41
// Player (header)
// (c) 2018 Jani Nykänen

#ifndef __PLAYER__
#define __PLAYER__

#include "../include/system.h"
#include "../include/renderer.h"
#include "../include/std.h"

// Player
typedef struct {

    VEC2 pos;
    VEC2 speed;
    VEC2 target;

    VEC2 tpos;

    bool moving;
    int flip;

    SPRITE spr;
}
PLAYER;

// Create a player object
PLAYER create_player(VEC2 pos, ASSET_PACK* ass);

// Update player
void pl_update(PLAYER* pl, float tm);

// Draw player
void pl_draw(PLAYER* pl);

#endif // __PLAYER__
