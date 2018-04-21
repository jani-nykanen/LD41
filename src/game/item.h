// LD41
// Item (header)
// (c) 2018 Jani Nykänen

#ifndef __ITEM__
#define __ITEM__

#include "../include/renderer.h"
#include "../include/system.h"

#include "player.h"

// Item type
typedef struct {

    VEC2 pos;
    int type;
    SPRITE spr;
    bool exist;
    bool active;

    float waveTimer;
}
ITEM;

// Initialize items
void init_items(ASSET_PACK* ass);

// Create an item
ITEM create_item(POINT p, int type);

// Update item
void item_update(ITEM* it, PLAYER* pl, float tm);

// Draw itme
void item_draw(ITEM* it);

#endif // __ITEM__
