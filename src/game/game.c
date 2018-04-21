// LD41
// Game scene (source)
// (c) 2018 Jani Nykänen

#include "game.h"

#include "camera.h"
#include "stage.h"

#include "../include/std.h"
#include "../include/renderer.h"
#include "../include/system.h"
#include "../include/utility.h"

#include "../global.h"

// Bitmaps
static BITMAP* bmpFont;

// Game canvas
static BITMAP* gameCanvas;


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
    stage_init(p);

    return 0;
}


// Update
static void game_update(float tm) {

    // Update stage
    stage_update(tm);
}


// Draw to canvas
static void draw_to_canvas() {

    clear(0);

    // Use global camera
    use_global_camera();

    // Draw stage
    stage_draw();
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
    int x = 320-236;
    int y = 12;

    fill_rect(x-3,y-3,gameCanvas->width +6, gameCanvas->height +6, 0b01101101);
    fill_rect(x-2,y-2,gameCanvas->width +4, gameCanvas->height +4, 255);
    fill_rect(x-1,y-1,gameCanvas->width +2, gameCanvas->height +2, 0);
    draw_bitmap_fast(gameCanvas, x,y);
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
