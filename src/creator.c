// LD41
// Creator scene (header)
// (c) 2018 Jani Nykänen

#include "creator.h"

#include "global.h"

#include "include/renderer.h"
#include "include/system.h"
#include "include/std.h"

// Bitmaps
static BITMAP* bmpCreator;
static BITMAP* bmpFont;

// Timer
static float timer;


// Initialize creator
static int creator_init() {

    ASSET_PACK* ass = global_get_asset_pack();

    // Get bitmaps
    bmpCreator = (BITMAP*)assets_get(ass, "creator");
    bmpFont = (BITMAP*)assets_get(ass, "font");

    timer = 0.0f;

    fade(-1, 1.0f, NULL);

    return 0;
}


// Swap to title
static void swap_to_title() {

    core_swap_scene("title");
}


// Update creator
static void creator_update(float tm) {

    if(is_fading()) return;

    timer += 1.0f * tm;

    if(input_get_mouse_button(1) == STATE_PRESSED || input_get_mouse_button(3) == STATE_PRESSED) {

        timer = 120.0f;
    }

    if(timer >= 120.0f) {

        fade(1, 2.0f, swap_to_title);
    }
}


// Draw creator
static void creator_draw() {

    clear(0);

    draw_bitmap_fast(bmpCreator,160-bmpCreator->width/2, 100-bmpCreator->height/2);

    draw_text(bmpFont,"Made in 48 hours for Ludum Dare 41",160,200-14,-7,0, true);
}


// Get creator scene
SCENE get_creator_scene() {

    return scene_create(
        creator_init,creator_update,creator_draw,NULL, NULL,
        "creator");
}
