// LD41
// Ending scene (header)
// (c) 2018 Jani Nykänen

#include "ending.h"

#include "global.h"

#include "include/renderer.h"
#include "include/system.h"
#include "include/std.h"

// Constants
static const float END_PHASE = 60.0f;
static const float PHASE_COUNT = 3;

// Font
static BITMAP* bmpFont;

// Ending timer
static float endTimer;
// Ending pahse
static int endPhase;


// Initialize ending
static int end_init() {

    ASSET_PACK* ass = global_get_asset_pack();

    // Get font
    bmpFont = (BITMAP*)assets_get(ass, "font");

    endTimer = 0.0f;
    endPhase = 0;

    return 0;
}


// Swap to game
static void swap_to_game() {

    core_swap_scene("game");
}


// Update ending
static void end_update(float tm) {

    endTimer += 1.0f * tm;

    if(endPhase < PHASE_COUNT+6 && endTimer >= END_PHASE) {

        endTimer -= END_PHASE;
        ++ endPhase;

        if(endPhase == 1) {

            fade(1,1.0f, NULL);
        }
    }

    if(endPhase >= PHASE_COUNT) {

        if(endPhase == PHASE_COUNT + 6 || ( !is_fading() && input_get_mouse_button(1) == STATE_PRESSED) ) {

            fade(1,2.0f, swap_to_game);
        }
    }
}


// Draw ending
static void end_draw() {

    if(endPhase == 0) {

        int t = (int)(160.0f * (endTimer / 60.0f));
        fill_rect(0,0,t,200,255);
        fill_rect(320-t,0,t,200,255);
    }
    else if(endPhase == 1) {

        clear(255);
    }
    else {

        clear(0);
    }


    if(endPhase >= 2) {

        draw_text(bmpFont, "CONGRATULATIONS!",160,32, -7,0, true);

        draw_text(bmpFont, "You used the teleporter to escape",160,56, -7,0, true);
        draw_text(bmpFont, "the caverns. You was teleported",160,68, -7,0, true);
        draw_text(bmpFont, "back to your home planet where",160,80, -7,0, true);
        draw_text(bmpFont, "a pack of wolves killed you.",160,92, -7,0, true);

        draw_text(bmpFont, "THE END",160,128, -7,0, true);
    }
}


// On change
static void end_on_change() {

    endTimer = 0.0f;
    endPhase = 0;
}


// Get ending scene
SCENE get_ending_scene() {

    return scene_create(
        end_init,end_update,end_draw,NULL, end_on_change,
        "ending");
}