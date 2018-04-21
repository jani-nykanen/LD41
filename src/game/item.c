// LD41
// Item (source)
// (c) 2018 Jani Nykänen

#include "item.h"

#include "status.h"
#include "camera.h"

// Item bitmap
static BITMAP* bmpGem =NULL;


// Initialize items
void init_items(ASSET_PACK* ass) {
    
    // Get assets
    bmpGem = (BITMAP*)assets_get(ass, "gem");
}


// Create an item
ITEM create_item(POINT p, int type) {

    ITEM it;
    it.pos = vec2(p.x*16.0f, p.y*16.0f);
    it.type = type;
    it.spr = create_sprite(16, 16);

    if(type < 4)
        it.spr.row = 0;
    else if(type == 4)
        it.spr.row = 1;
    else if(type == 5)
        it.spr.row = 2;
    else if(type == 6)
        it.spr.row = 3;


    it.waveTimer = 0.0f;
    if(type == 5 || type == 6)
        it.waveTimer = (float)(rand() % 1000) / 1000.0f * 2* M_PI;

    it.active = true;
    it.exist = true;

    return it;
}


// Update item
void item_update(ITEM* it, PLAYER* pl, float tm) {

    const float WAVE_SPEED = 0.05f;

    if(it->exist == false) return;

    // Is active
    VEC2 cpos = get_global_camera()->pos;

    it->active = !(it->pos.x+32 < cpos.x || it->pos.x-16 > cpos.x+224 
        || it->pos.y+32 < cpos.y || it->pos.y-16 > cpos.y+176);

    // Animate
    if(it->type < 5) {

        spr_animate(&it->spr,it->type == 4 ? 1: 0, 0,2, 6, tm);
    }
    else if(it->type == 5 || it->type == 6) {

        if(it->type == 5)
            spr_animate(&it->spr, 2, 0,7, 8, tm);

        it->waveTimer += WAVE_SPEED* tm;
    }

    // Collision
    VEC2 p1 = pl->pos;
    VEC2 p2 = it->pos;
    const float DIM = 6;
    
    if(p1.x+DIM > p2.x && p1.x-DIM < p2.x+16 && p1.y+DIM > p2.y && p1.y-DIM < p2.y+16) {

        it->exist = false;
        
        STATUS* st = get_status();
        switch(it->type) {
        case 0:
        case 1:
        case 2:
        case 3:
            st->items[it->type] = true;
            ++ st->redCount; break;

        case 4:
            ++ st->blueCount; break;

        case 5:
            st->time += 15.0f * 60.0f; break;

        case 6:
            ++ st->ammo; break;

        default:
            break;

        }
    }
    
}


// Draw itme
void item_draw(ITEM* it) {

    if(it->exist == false || it->active == false) 
        return;

    spr_draw(&it->spr, bmpGem, (int)it->pos.x, (int)it->pos.y + (int) (sin(it->waveTimer) * 3.0f), 0);
}
