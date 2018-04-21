// LD41
// Game scene (source)
// (c) 2018 Jani Nykänen

#include "game.h"

#include "camera.h"
#include "stage.h"
#include "player.h"

#include "../include/std.h"
#include "../include/renderer.h"
#include "../include/system.h"
#include "../include/utility.h"

#include "../global.h"
#include "../keyconfig.h"

// Constants
static const int CANVAS_X = 12;
static const int CANVAS_Y = 12;

// Bitmaps
static BITMAP* bmpFont;

// Game canvas
static BITMAP* gameCanvas;

// Game objects
static PLAYER pl;


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


// Initialize
static int game_init() {

    // Get assets
    ASSET_PACK* p = global_get_asset_pack();
    bmpFont = (BITMAP*)assets_get(p, "font");

    // Create canvas
    gameCanvas = bitmap_create(224, 176);
    if(gameCanvas == NULL) {

        return 1;
    }

    // Initialize components
    init_global_camera();

    if(stage_init(p) == 1)
        return 1;

    pl = create_player(vec2(35*16, 28*16-8), p);

    return 0;
}


// Update
static void game_update(float tm) {

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

    }
}


// Draw to canvas
static void draw_to_canvas() {

    clear(0);

    // Use global camera
    use_global_camera();

    // Draw stage
    stage_draw();

    // Draw player
    pl_draw(&pl);

}


// Draw
static void game_draw() {

    // Clear screen
    clear(0);

    // Set target to the canvas
    set_render_target(gameCanvas),
    draw_to_canvas();
    set_render_target(NULL);

    // Draw canvas
    translate(0, 0);
    int x = CANVAS_X;
    int y = CANVAS_Y;

    fill_rect(x-3,y-3,gameCanvas->width +6, gameCanvas->height +6, 0b01101101);
    fill_rect(x-2,y-2,gameCanvas->width +4, gameCanvas->height +4, 255);
    fill_rect(x-1,y-1,gameCanvas->width +2, gameCanvas->height +2, 0);
    draw_bitmap_fast(gameCanvas, x,y);

    // Draw map
    draw_text(bmpFont,"MAP:",320-72 +24,8,-7,0,true);
    stage_draw_map(320-72 +4,12 +12, pl.pos);
}


// Destroy
static void game_destroy() {

}


// Change
static void game_on_change() {

}


// Get scene
SCENE game_get_scene() {

    return scene_create(
        game_init,game_update,game_draw,game_destroy,game_on_change,
        "game");
}
