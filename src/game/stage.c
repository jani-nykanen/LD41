// LD41
// Stage (source)
// (c) 2018 Jani Nykänen

#include "stage.h"

#include "camera.h"

#include "../include/renderer.h"
#include "../include/std.h"

#include "../lib/tmxc.h"

// Bitmaps
static BITMAP* bmpTileset;
static BITMAP* bmpMap =NULL;

// Tilemap
static TILEMAP* mapBase;

// Tile data
static int* tiledata =NULL;



// Generate map
static int gen_map() {

    if(bmpMap == NULL) {

        bmpMap = bitmap_create(mapBase->width, mapBase->height);
        if(bmpMap == NULL) {

            return 1;
        }
    }

    int i = 0;
    int v = 0;
    for(; i < mapBase->width*mapBase->height; ++ i) {

        v = tiledata[i];
        if(v == 0 || v > 32) {

            bmpMap->data[i] = 255;
        }
        else {

            bmpMap->data[i] = 0;
        }

    }

    return 0;
}


// Draw tilemap area
static void draw_tilemap_area(TILEMAP* t, int sx, int sy, int ex, int ey) {

    int x = sx;
    int y = sy;
    int tx = 0;
    int ty = 0;

    int tile;

    for(; y < ey; ++ y) {

        for(x = sx; x < ex; ++ x) {

            if(x < 0 || y < 0 || x >= t->width || y >= t->height)
                continue;

            tile = tiledata[y*t->width + x];
            if(tile == 0) continue;

            -- tile;

            tx = tile % 16;
            ty = tile / 16;

            draw_bitmap_region_fast(bmpTileset, tx*16,ty*16,16,16, x*16, y*16);
        }
    }
}


// Initialize stage
int stage_init(ASSET_PACK* ass) {

    // Get assets
    bmpTileset = (BITMAP*)assets_get(ass, "tileset");

    mapBase = (TILEMAP*)assets_get(ass, "base");
    if(tiledata != NULL)
        free(tiledata);

    if(tiledata == NULL)
        tiledata = (int*)malloc(sizeof(int) * mapBase->width * mapBase->height);

    if(tiledata == NULL) {

        error_mem_alloc();
        return 1;
    }

    // Copy
    int i = 0;
    for(; i < mapBase->width * mapBase->height; ++ i) {

        tiledata[i] = mapBase->layers[0][i];
    }

    // Generate map
    if(gen_map() == 1) {

        return 1;
    }

    return 0;
}


// Update stage
void stage_update(float tm) {

}


// Player collision
void stage_pl_collision(PLAYER* pl, float tm) {

    int px = (int)floor(pl->pos.x / 16.0);
    int py = (int)floor(pl->pos.y / 16.0);

    int x,y;
    int tile;
    int t2;

    int w = mapBase->width;
    int h = mapBase->height;

    for(y=py-2; y <= py+2; ++ y) {

        for(x=px-2; x <= px+2; ++ x) {

            if(x < 0 || y < 0 || x >= mapBase->width || y >= mapBase->height)
                continue;

            tile = tiledata[y * w+x];
            if(tile == 0 || tile > 32)
                continue;

            t2 = y > 0? tiledata[(y-1)*w+x] : 0;
            if(t2 == 0 || t2 > 32)
                pl_surface_collision(pl, COL_DOWN, x*16, y*16, 16, tm);

            t2 = y < h-1? tiledata[(y+1)*w+x] : 0;
            if(t2 == 0 || t2 > 32)
                pl_surface_collision(pl, COL_UP, x*16, y*16 +16, 16, tm);

            t2 = x > 0 ? tiledata[y*w+x-1] : 0;
            if(t2 == 0 || t2 > 32)
                pl_surface_collision(pl, COL_RIGHT, x*16, y*16, 16, tm);

            t2 = x < w-1 ? tiledata[y*w+x+1] : 0;
            if(t2 == 0 || t2 > 32)
                pl_surface_collision(pl, COL_LEFT, x*16+16, y*16, 16, tm);
            
        }
    }
}


// Draw stage
void stage_draw() {

    // Get camera pos
    CAMERA* c = get_global_camera();

    int sx = (int)floor(c->pos.x / 16.0) -1;
    int sy = (int)floor(c->pos.y / 16.0) -1;
    int ex = sx + 16;
    int ey = sy + 13;

    // Draw tilemap area
    draw_tilemap_area(mapBase,sx,sy,ex,ey);
}


// Draw map
void stage_draw_map(int x, int y, VEC2 p) {

    fill_rect(x-2,y-2,bmpMap->width+4,bmpMap->height+4, 0b01101101);
    fill_rect(x-1,y-1,bmpMap->width+2,bmpMap->height+2, 255);
    draw_bitmap_fast(bmpMap, x, y);

    // Player
    int px = (int)floor(p.x / 16.0f);
    int py = (int)floor(p.y / 16.0f);

    fill_rect(x +px-1, y+py-1,3,3, 0b11100000);
}
