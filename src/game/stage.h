// LD41
// Stage (header)
// (c) 2018 Jani Nykänen

#ifndef __STAGE__
#define __STAGE__

#include "../include/system.h"

// Initialize stage
void stage_init(ASSET_PACK* ass);

// Update stage
void stage_update(float tm);

// Draw stage
void stage_draw();

#endif // __STAGE__
