// LD41
// Main (source)
// (c) 2018 Jani Nykänen

#include "engine/core.h"

// Scenes
#include "game/game.h"
#include "global.h"

// Main
int main(int argc, char** argv) {
    
    // Add scenes
    core_add_scene(global_get_scene());
    core_add_scene(game_get_scene());
    
    return core_run_application("properties.conf");
}
