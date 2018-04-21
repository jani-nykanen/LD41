// LD41
// Stage (header)
// (c) 2018 Jani Nykänen

#ifndef __STAGE__
#define __STAGE__

#include "../include/system.h"

#include "player.h"

// Initialize stage
int stage_init(ASSET_PACK* ass);

// Update stage
void stage_update(float tm);

// Player collision
void stage_pl_collision(PLAYER* pl, float tm);

// Draw stage
void stage_draw(VEC2 p);

// Draw map
void stage_draw_map(int x, int y, VEC2 ppos);

#endif // __STAGE__
