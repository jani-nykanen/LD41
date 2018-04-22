// LD41
// Global scene (source)
// (c) 2018 Jani Nykänen

#include "global.h"

#include "keyconfig.h"

#include "include/std.h"
#include "include/renderer.h"
#include "include/system.h"

// Global assets
static ASSET_PACK* globalAssets;

// Cursor bitmap
static BITMAP* bmpCursor;
// Cursor mode
static int cursorMode;

// Fade timer
static float fadeTimer;
// Fade mode
static int fadeMode;
// Fade callback
static void (*fadeCb) (void) = NULL;
// Fade speed
static float fadeSpeed;


// Get the global assets
ASSET_PACK* global_get_asset_pack() {

    return globalAssets;
}


// Set cursor mode
void set_cursor_mode(int m) {

    cursorMode = m;
}


// Get the cursor mode
int get_cursor_mode() {

    return cursorMode;
}


// Initialize
static int global_init() {

    CONFIG c = core_get_config();

    // Load global assets
    globalAssets = load_asset_pack(c.assetPath);
    if(globalAssets == NULL) {

        return 1;   
    }

    // Get assets
    bmpCursor = (BITMAP*)assets_get(globalAssets, "cursor");

    // Initialize default key configuration
    key_init_default();

    // Set default values
    cursorMode = 0;
    fadeMode = 0;
    fadeTimer = 0.0f;
    fadeSpeed = 2.0f;

    return 0;
}


// Update
static void global_update(float tm) {

    // Update fading
    if(fadeMode != 0) {

        fadeTimer += fadeSpeed * tm;
        if(fadeTimer >= 60.0f) {
            
            if(fadeMode == 1) {

                fadeMode = -1;
                fadeTimer = 0.0f;

                if(fadeCb != NULL)
                    fadeCb();
            }
            else {

                fadeMode = 0;
                fadeTimer = 0.0f;
            }
        }
    }
}


// Draw
static void global_draw() {

    // Draw fading
    if(fadeMode != 0 ){

        int dvalue = 0;
        if(fadeMode == 1) {

            dvalue = (int)(fadeTimer/60.0f * 14.0f);
        }
        else {

            dvalue = (int)((1.0f-fadeTimer/60.0f) * 14.0f);
        }

        darken(dvalue);
    }

    // Draw cursor
    POINT cpos = input_get_cursor_pos();
    int sx = cursorMode*12;
    draw_bitmap_region(bmpCursor,sx,0,12,18,cpos.x,cpos.y, FLIP_NONE);
}


// Destroy
static void global_destroy() {

    assets_destroy(globalAssets);
}


// Fade
void fade(int dir, float speed, void (*cb)(void)) {

    fadeCb = cb;
    fadeMode = dir;
    fadeSpeed = speed;
}


// Is fading
bool is_fading() {

    return fadeMode != 0;
}


// Get the global scene
SCENE global_get_scene() {

    return scene_create(global_init,global_update,global_draw,global_destroy,NULL,
        "global");
}
