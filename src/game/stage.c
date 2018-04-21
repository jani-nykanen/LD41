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

// Tilemap
static TILEMAP* mapBase;


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

            tile = t->layers[0][y*t->width + x];
            if(tile == 0) continue;

            -- tile;

            tx = tile % 16;
            ty = tile / 16;

            draw_bitmap_region_fast(bmpTileset, tx*16,ty*16,16,16, x*16, y*16);
        }
    }
}


// Initialize stage
void stage_init(ASSET_PACK* ass) {

    // Get assets
    bmpTileset = (BITMAP*)assets_get(ass, "tileset");

    mapBase = (TILEMAP*)assets_get(ass, "base");
}


// Update stage
void stage_update(float tm) {

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
