// LD41
// Game scene (source)
// (c) 2018 Jani Nykänen

#include "game.h"

#include "camera.h"
#include "stage.h"
#include "player.h"
#include "status.h"
#include "item.h"
#include "enemy.h"
#include "splash.h"

#include "../include/std.h"
#include "../include/system.h"
#include "../include/utility.h"

#include "../global.h"
#include "../keyconfig.h"

// Constants
static const int CANVAS_X = 12;
static const int CANVAS_Y = 12;
static const float GOVER_MAX = 150.0f;
#define  ITEM_COUNT 64
#define ENEMY_COUNT 64
#define SPLASH_COUNT 8

// Bitmaps
static BITMAP* bmpFont;
static BITMAP* bmpGem;
static BITMAP* bmpTimesUp;
static BITMAP* bmpSplash;

// Game canvas
static BITMAP* gameCanvas;

// Game objects
static PLAYER pl;
static ITEM items[ITEM_COUNT];
static ENEMY enemies[ENEMY_COUNT];
static SPLASH splashes[SPLASH_COUNT];

// Item text timer
static float itemTextTimer;
// Item text id
static int itemTextID;

// Is the game over
static bool gameOver;
// Game over timer
static float goverTimer;


// Reset game
static int game_reset(ASSET_PACK* ass) {

    srand(0);

    // Initialize components
    init_global_camera();
    reset_status();

    pl = create_player(vec2(35*16, 28*16-8), ass);

    int i = 0;
    for(; i < ITEM_COUNT; ++ i) {

        items[i].exist = false;
    }

    for(i=0; i < ENEMY_COUNT; ++ i) {

        enemies[i].exist = false;
    }

    for(i=0; i < SPLASH_COUNT; ++ i) {

        splashes[i].exist = false;
    }

    if(stage_init(ass) == 1)
        return 1;

    // Set default values
    gameOver = false;
    goverTimer = 0.0f;

    // Fade
    fade(-1,2.0f, NULL);

    return 0;
}


// Reset callback
static void reset_cb() {

    game_reset(NULL);
}


// Update game over
static void update_game_over(float tm) {

    goverTimer += 1.0f * tm;
    if(goverTimer >= 120.0f && !is_fading()) {

        fade(1,2.0f, reset_cb);
    }
}


// Draw game over
static void draw_game_over() {

    draw_bitmap_fast(bmpTimesUp, 16+224/2-bmpTimesUp->width/2,64);
}


// Set the cursor mode
static void game_set_cursor_mode() {

    POINT cpos = input_get_cursor_pos();

    cpos.x += 6;
    cpos.y += 6;

    int mode = 0;

    if(cpos.x >= 248-6) {

        mode = 0;
    }
    else if(cpos.x > CANVAS_X && cpos.x < CANVAS_X + gameCanvas->width && cpos.y < CANVAS_Y+12) {

        mode = 1;
    }
    else if(cpos.x > CANVAS_X && cpos.x < CANVAS_X + gameCanvas->width 
        && cpos.y > CANVAS_Y + gameCanvas->height-12) {

        mode = 3;
    }
    else if(cpos.y > CANVAS_Y && cpos.y < CANVAS_Y + gameCanvas->height && cpos.x < CANVAS_X+12) {

        mode = 4;
    }
    else if(cpos.y > CANVAS_Y && cpos.y < CANVAS_Y + gameCanvas->height 
        && cpos.x > CANVAS_X + gameCanvas->width-12) {

        mode = 2;
    }

    set_cursor_mode(mode);
}


// Draw item text
static void draw_item_text() {

    const char* text[] = {
        "You learned DIG!",
        "You learned\nNIGHT VISION!",
        "You learned\nSUPER DIG!",
        "You learned SWIM!"
    };

    if(itemTextID == 0 || itemTextID == 3)
        draw_text(bmpFont,text[itemTextID],224/2,32,-7,0,true);
    else
        draw_text(bmpFont,text[itemTextID],224/2 -60,32,-7,0,false);
}


// Initialize
static int game_init() {

    // Get assets
    ASSET_PACK* p = global_get_asset_pack();
    bmpFont = (BITMAP*)assets_get(p, "font");
    bmpGem = (BITMAP*)assets_get(p, "gem");
    bmpTimesUp = (BITMAP*)assets_get(p,"timeup");
    bmpSplash = (BITMAP*)assets_get(p,"splash");

    // Create canvas
    gameCanvas = bitmap_create(224, 176);
    if(gameCanvas == NULL) {

        return 1;
    }

    // Initialize components
    init_items(p);
    init_splash(p);
    
    game_reset(p);

    return 0;
}


// Update
static void game_update(float tm) {

    if(gameOver) {

        set_cursor_mode(0);
        update_game_over(tm);

        return;
    }

    int i = 0;

    // Set cursor mode
    game_set_cursor_mode();

    // Update camera
    update_camera(tm);

    if(!get_global_camera()->moving) {

        // Update player
        pl_update(&pl, tm);

        // Update stage
        stage_update(tm);
        stage_pl_collision(&pl, tm);

        // Update status
        status_update(tm);

        // If no time, game over
        if(get_status()->time <= 0.0f) {

            gameOver = true;
            goverTimer = 0.0f;
        }
 
    }

    // Update items
    for(i=0; i < ITEM_COUNT; ++ i) {

        item_update(&items[i], &pl, tm);
    }

    // Update enemies
    for(i=0; i < ENEMY_COUNT; ++ i) {

        enemy_update(&enemies[i], &pl, tm);
    }

    // Update splashes
    for(i=0; i < SPLASH_COUNT; ++ i) {

        splash_update(&splashes[i], tm);
    }

    // Update item text
    if(itemTextTimer > 0.0f) {

        itemTextTimer -= 1.0f * tm;
    }

    // Shoot
    if(get_status()->ammo > 0 && input_get_mouse_button(3) == STATE_PRESSED) {

        POINT c = input_get_cursor_pos();
        if(c.x >= CANVAS_X && c.x <= CANVAS_X + gameCanvas->width
         && c.y >= CANVAS_Y && c.y <= CANVAS_Y + gameCanvas->height) {
          
            int i = 0;
            for(; i < SPLASH_COUNT; ++ i) {

                if(!splashes[i].exist) {
                    
                    splashes[i] = create_splash(vec2(c.x-CANVAS_X, c.y-CANVAS_Y));
                    break;
                }
            }

            -- get_status()->ammo;

        }
    }

    if(input_get_key((int)SDL_SCANCODE_P) == STATE_PRESSED) {

        core_swap_scene("ending");
    }
}


// Draw to canvas
static void draw_to_canvas() {

    clear(0);

    // Use global camera
    use_global_camera();

    // Draw stage
    stage_draw(pl.pos);

    // Draw items
    int i = 0;
    for(; i < ITEM_COUNT; ++ i) {

        item_draw(&items[i]);
    }

    // Draw player
    pl_draw(&pl);

    // Draw enemies
    for(i = 0; i < ENEMY_COUNT; ++ i) {

        enemy_draw(&enemies[i]);
    }

    
    // Draw splashes
    for(i=0; i < SPLASH_COUNT; ++ i) {

        translate(0, 0);
        splash_draw(&splashes[i]);
    }

    // Draw item text
    if(itemTextTimer > 0.0f) {

        translate(0, 0);
        draw_item_text();
    }
}


// Draw status
static void draw_status() {

    // Draw time
    char str[16];
    status_get_time_string(str);

    if(pl.hurtTimer <= 0 || pl.spr.frame == 0)
        draw_text(bmpFont, str, 264,93,-8,0, false);

    // Draw icons
    draw_bitmap_region(bmpGem,0,32,16,16,248,92, 0);
    draw_bitmap_region(bmpGem,0,48,16,16,248,92 + 24, 0);
    draw_bitmap_region(bmpGem,0,16,16,16,248,92 + 48, 0);
    draw_bitmap_region(bmpGem,0,0,16,16,248,92 + 72, 0);

    // Draw amounts
    STATUS* st = get_status();
    snprintf(str,16,"x%d",st->ammo);
    draw_text(bmpFont, str, 264,93 +24,-7,0, false);

    snprintf(str,16,"%d/8",st->blueCount);
    draw_text(bmpFont, str, 264,93 +48,-7,0, false);

    snprintf(str,16,"%d/4",st->redCount);
    draw_text(bmpFont, str, 264,93 +72,-7,0, false);
}


// Draw
static void game_draw() {

    // Clear screen
    clear(0b01100100);

    // Set target to the canvas
    set_render_target(gameCanvas),
    draw_to_canvas();
    set_render_target(NULL);

    // Draw canvas
    translate(0, 0);
    int x = CANVAS_X;
    int y = CANVAS_Y;

    fill_rect(x-3,y-3,gameCanvas->width +6, gameCanvas->height +6, 0);
    fill_rect(x-2,y-2,gameCanvas->width +4, gameCanvas->height +4, 0b01101101);
    fill_rect(x-1,y-1,gameCanvas->width +2, gameCanvas->height +2, 255);
    draw_bitmap_fast(gameCanvas, x,y);

    // Draw map
    draw_text(bmpFont,"MAP:",320-72 +36,8,-6,0,true);
    stage_draw_map(320-72 +4,12 +12, pl.pos);

    draw_status();

    // If game over, draw "Time's Up!"
    if(gameOver) {

        draw_game_over();
    }

}


// Destroy
static void game_destroy() {

}


// Change
static void game_on_change() {

    game_reset(NULL);
}


// Get scene
SCENE game_get_scene() {

    return scene_create(
        game_init,game_update,game_draw,game_destroy,game_on_change,
        "game");
}


// Add an item
void add_item(POINT p, int type) {

    int i = 0;
    ITEM* it;
    for(; i < ITEM_COUNT; ++ i) {

        it = &items[i];
        if(!it->exist) {

            *it = create_item(p, type);
            return;
        }
    }
}


// Add an enemy
void add_enemy(POINT p) {

    int i = 0;
    ENEMY* e;
    for(; i < ENEMY_COUNT; ++ i) {

        e = &enemies[i];
        if(!e->exist) {

            *e = create_enemy(vec2(p.x*16.0f + 8.0f, p.y*16.0f + 8.0f));
            return;
        }
    }
}


// Set item text
void set_item_text(int id) {

    itemTextTimer = 150.0f;
    itemTextID = id;

}


// Is shooting
bool can_shoot() {

    return get_status()->ammo > 0;
}
