// LD41
// Player (header)
// (c) 2018 Jani Nykänen

#ifndef __PLAYER__
#define __PLAYER__

#include "../include/system.h"
#include "../include/renderer.h"
#include "../include/std.h"

// Collision types
enum {

    COL_RIGHT = 0,
    COL_UP = 1,
    COL_LEFT = 2,
    COL_DOWN = 3,
};

// Player
typedef struct {

    VEC2 pos;
    VEC2 speed;
    VEC2 target;

    VEC2 tpos;

    bool moving;
    int flip;

    SPRITE spr;
    SPRITE tspr;
    SPRITE pspr;
    float pspeed;
    float hurtTimer;
}
PLAYER;

// Create a player object
PLAYER create_player(VEC2 pos, ASSET_PACK* ass);

// Update player
void pl_update(PLAYER* pl, float tm);

// Draw player
void pl_draw(PLAYER* pl);

// Surface collision
bool pl_surface_collision(PLAYER* pl, int type, int x, int y, int d, float tm);

// Hurt collision
bool pl_hurt_collision(PLAYER* pl, int x, int y, int w, int h);

#endif // __PLAYER__
