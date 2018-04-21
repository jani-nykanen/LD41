// LD41
// Global scene (header)
// (c) 2018 Jani Nykänen

#ifndef __GLOBAL__
#define __GLOBAL__

#include "engine/scene.h"
#include "engine/assets.h"

// Get assets
ASSET_PACK* global_get_asset_pack();

// Set cursor mode
void set_cursor_mode(int m);

// Get the global scene
SCENE global_get_scene();

#endif // __GLOBAL__
