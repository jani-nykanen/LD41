// LD41
// Game scene (header)
// (c) 2018 Jani Nykänen

#ifndef __GAME__
#define __GAME__

#include "../engine/scene.h"

#include "../include/renderer.h"

// Get game scene
SCENE game_get_scene();

// Add an item
void add_item(POINT p, int type);

#endif // __GAME__
