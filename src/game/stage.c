// LD41
// Stage (source)
// (c) 2018 Jani Nykänen

#include "stage.h"

#include "camera.h"
#include "status.h"
#include "game.h"

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
// Visited tiles
static bool visitedTiles[4*4]; 



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

            bmpMap->data[i] = 0b01001001;
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
            if(tile == 0 || tile > 32) continue;

            -- tile;

            tx = tile % 16;
            ty = tile / 16;

            draw_bitmap_region_fast(bmpTileset, tx*16,ty*16,16,16, x*16, y*16);
        }
    }
}


// Is tile hurt tile
static bool is_hurt_tile(int t) {

    return (t >= 8 && t <= 12);
}


// Is tile not solid
static bool is_not_solid(int t) {

    STATUS* st = get_status();
    return (is_hurt_tile(t) || t == 0 || t > 32 || (st->items[1] && (t == 7 || t == 23)));
}


// Add items
static void add_items() {

    int x,y;
    int t;
    for(y = 0; y < mapBase->height; ++ y) {

        for(x = 0; x < mapBase->width; ++ x) {

            t = tiledata[y*mapBase->width + x];
            if(t > 32 && t <= 39) {

                add_item(point(x,y), t-33);
            }
            else if(t == 40) {

                add_enemy(point(x,y));
            }

        }

    }

}


// Initialize stage
int stage_init(ASSET_PACK* ass) {

    // Get assets
    if(ass != NULL) {

        bmpTileset = (BITMAP*)assets_get(ass, "tileset");
        mapBase = (TILEMAP*)assets_get(ass, "base");
    }

    if(tiledata == NULL) {

        tiledata = (int*)malloc(sizeof(int) * mapBase->width * mapBase->height);

        if(tiledata == NULL ) {

            error_mem_alloc();
            return 1;
        }
    }

    // Copy
    int i = 0;
    for(; i < mapBase->width * mapBase->height; ++ i) {

        tiledata[i] = mapBase->layers[0][i];
        if(i < 4*4)
            visitedTiles[i] = false;
    }

    // Generate map
    if(gen_map() == 1) {

        return 1;
    }

    // Add items
    add_items();

    return 0;
}


// Update stage
void stage_update(float tm) {

    VEC2 cpos = get_global_camera()->pos;

    int x = (int)floor(cpos.x / 224);
    int y = (int)floor(cpos.y / 176);

    visitedTiles[y * 4 + x] = true;
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

    bool col[4];
    int i = 0;
    for(; i < 4; ++ i)
        col[i] = false;

    STATUS* st = get_status();

    for(y=py-2; y <= py+2; ++ y) {

        for(x=px-2; x <= px+2; ++ x) {

            if(x < 0 || y < 0 || x >= mapBase->width || y >= mapBase->height)
                continue;

            tile = tiledata[y * w+x];

            if(is_hurt_tile(tile)) {

                pl_hurt_collision(pl,x*16+2,y*16+2,12,12);
                continue;
            }

            if(is_not_solid(tile))
                continue;

            t2 = y > 0? tiledata[(y-1)*w+x] : 0;
            if(is_not_solid(t2))
                col[0] = pl_surface_collision(pl, COL_DOWN, x*16, y*16, 16, tm);

            t2 = y < h-1? tiledata[(y+1)*w+x] : 0;
            if(is_not_solid(t2))
                col[1] = pl_surface_collision(pl, COL_UP, x*16, y*16 +16, 16, tm);

            t2 = x > 0 ? tiledata[y*w+x-1] : 0;
            if(is_not_solid(t2))
                col[2] = pl_surface_collision(pl, COL_RIGHT, x*16, y*16, 16, tm);

            t2 = x < w-1 ? tiledata[y*w+x+1] : 0;
            if(is_not_solid(t2))
                col[3] = pl_surface_collision(pl, COL_LEFT, x*16+16, y*16, 16, tm);

            if( (st->items[0] && tile == 22) || (st->items[3] && tile == 24) ) {

                for(i=0; i < 4; ++i) {

                    if(col[i]) {

                        tiledata[y*w+x] = 0;
                    }
                }
            }
            
        }
    }
}


// Draw stage
void stage_draw(VEC2 p) {

    // Get camera pos
    CAMERA* c = get_global_camera();

    int sx = (int)floor(c->pos.x / 16.0) -1;
    int sy = (int)floor(c->pos.y / 16.0) -1;
    int ex = sx + 16;
    int ey = sy + 13;

    // Draw tilemap area
    draw_tilemap_area(mapBase,sx,sy,ex,ey);

    if( (c->target.x == 0 && c->target.y == 0) || (c->grid.x == 0 && c->grid.y == 0) ) {
        
        int x = 0;
        int y = 0;

        if(!get_status()->items[1]) {

            fill_rect(x,y,224,176, 0);
        }
        else {

            fill_rect(x,y,(int)p.x - 32 - x,176, 0);
            fill_rect((int)p.x + 32,y,x + 224 - (int)p.x + 32,176, 0);
            fill_rect(x,y,224,(int)p.y - 32 - y, 0);
            fill_rect(x,(int)p.y + 32,224,y + 176 - (int)p.y + 32, 0);

            draw_bitmap_region(bmpTileset,256-64,0,64,64, (int)p.x-32,(int)p.y-32, 0);
        }

    }

}


// Draw map
void stage_draw_map(int x, int y, VEC2 p) {

    fill_rect(x-3,y-3,bmpMap->width+6,bmpMap->height+6, 0);
    fill_rect(x-2,y-2,bmpMap->width+4,bmpMap->height+4, 255);
    fill_rect(x-1,y-1,bmpMap->width+2,bmpMap->height+2, 0b01101101);
    draw_bitmap_fast(bmpMap, x, y);

    // Player
    int px = (int)floor(p.x / 16.0f);
    int py = (int)floor(p.y / 16.0f);

    fill_rect(x +px-1, y+py-1,3,3, 0b11100000);

    int dx,dy;
    for(dy = 0; dy < 4; ++ dy) {

        for(dx = 0; dx < 4; ++ dx) {
            
            if(!visitedTiles[dy*4 + dx]) {

                fill_rect(x + dx*14, y + dy*11, 14,11, 0);
            }

        }
    }
}
