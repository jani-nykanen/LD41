// LD41
// Enemy (header)
// (c) 2018 Jani Nykäen

#ifndef __ENEMY__
#define __ENEMY__

#include "player.h"

#include "../include/renderer.h"
#include "../include/std.h"

// Item type
typedef struct {

    VEC2 pos;
    POINT grid;
    bool exist;
    bool active;
    bool dead;
    float angle;
    VEC2 speed;

    float radius;
    float triAngle; // Ehheh
    float size;

    float x1,y1,x2,y2,x3,y3;
}
ENEMY;

// Create an enemy
ENEMY create_enemy(VEC2 pos);

// Update an enemy
void enemy_update(ENEMY* e, PLAYER* pl, float tm);

// Draw an enemy
void enemy_draw(ENEMY* e);

#endif // __ENEMY__
