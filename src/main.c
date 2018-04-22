// LD41
// Main (source)
// (c) 2018 Jani Nykänen

#define SDL_MAIN_HANDLED

#include "engine/core.h"

// Scenes
#include "game/game.h"
#include "global.h"
#include "ending.h"
#include "creator.h"
#include "title.h"

// Main
int main(int argc, char** argv) {
    
    // Add scenes
    core_add_scene(global_get_scene());
    core_add_scene(get_ending_scene());
    core_add_scene(game_get_scene());
    core_add_scene(get_title_scene());
    core_add_scene(get_creator_scene());
    
    return core_run_application("properties.conf");
}
