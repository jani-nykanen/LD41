// LD41
// Title scene (header)
// (c) 2018 Jani Nykänen

#include "title.h"

#include "global.h"

#include "include/renderer.h"
#include "include/system.h"
#include "include/std.h"

// Bitmaps
static BITMAP* bmpLogo;
static BITMAP* bmpFont;
static BITMAP* bmpFont2;

// Phase
static int phase;
// Click timer
static float clickTimer;
// Selection
static int selection;

// Draw story
static void draw_story() {

    const char story[] =
        "    STORY:\n\n"
        "You are some random something from\n"
        "some random planet. For some reason\n"
        "you got stuck in some random cavern\n"
        "in some random planet. Or something\n"
        "like that. Anyway, you must find a\n"
        "teleporter and get back to your home\n"
        "planet.";

    draw_text(bmpFont,story,8,16,-8,0,false);
}


// Initialize title
static int title_init() {

    ASSET_PACK* ass = global_get_asset_pack();

    // Get bitmaps
    bmpLogo = (BITMAP*)assets_get(ass, "logo");
    bmpFont = (BITMAP*)assets_get(ass, "font");
    bmpFont2 = (BITMAP*)assets_get(ass, "font2");

    phase = 0;
    clickTimer = 0;

    return 0;
}


// Swap to game
static void swap_to_game() {

    core_swap_scene("game");
}


// Next phase
static void next_phase() {

    ++ phase;
}


// Update title
static void title_update(float tm) {

    if(is_fading()) return;

    if(phase == 0) {

        clickTimer += 1.0f * tm;
        if(clickTimer >= 60.0f)
            clickTimer -= 60.0f;
            
        if(input_get_mouse_button(1) == STATE_PRESSED 
        || input_get_mouse_button(3) == STATE_PRESSED) {

            ++ phase;
        }

    }
    else if(phase == 1) {

        if(input_get_mouse_button(1) == STATE_PRESSED 
        || input_get_mouse_button(3) == STATE_PRESSED) {

            if(selection == 0)
                fade(1,2.0f,next_phase);
            
            else if(selection == 1) 
                core_toggle_fullscreen();

            else if(selection == 2)
                fade(1,2.0f,core_terminate);
        }

        int cy = input_get_cursor_pos().y;

        if(cy < 200-64 || cy > 200-14) 
            selection = -1;

        else {

            selection =  (cy - (200-64) ) / 17;
        }

    }
    else {

        if(input_get_mouse_button(1) == STATE_PRESSED 
        || input_get_mouse_button(3) == STATE_PRESSED) {

            fade(1,2.0f,swap_to_game);
        }
    }

    
}


// Draw title
static void title_draw() {

    clear(0b01100100);

    draw_bitmap(bmpLogo,160-bmpLogo->width/2, 0, 0);

    // "Click to start"
    if(phase == 0) {

        if(!is_fading() && clickTimer < 30.0f)
            draw_text(bmpFont, "Click screen to start",160,200-48,-7,0,true);
    }
    else if(phase == 1) {

        draw_text(selection == 0 ? bmpFont2 : bmpFont, "Start game", 160-64, 200-64, -7,0, false);
        draw_text(selection == 1 ? bmpFont2 : bmpFont, "Full screen", 160-64, 200-64 +16, -7,0, false);
        draw_text(selection == 2 ? bmpFont2 : bmpFont, "Quit game", 160-64, 200-64 +32, -7,0, false);
    }

    // Copyright
    draw_text(bmpFont, "(c)2018 Jani Nyk~nen",160,200-14,-7,0,true);

    if(phase == 2) {

        clear(0);
        draw_story();
    }

    
}


// On change event
void title_on_change() {

    if(phase > 0) {

        phase = 1;
    }
}


// Get title scene
SCENE get_title_scene() {

    return scene_create(
        title_init,title_update,title_draw,NULL, title_on_change,
        "title");
}
