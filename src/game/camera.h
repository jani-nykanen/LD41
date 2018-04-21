// LD41
// Camera (header)
// (c) 2018 Jani Nykänen

#ifndef __CAMERA__
#define __CAMERA__

#include "../include/renderer.h"
#include "../include/std.h"

// Camera type
typedef struct {

    VEC2 pos;
    POINT grid; // Grid position
    POINT target; // Target grid
    bool moving;
    int step;
}
CAMERA;

// Initialize global camera
void init_global_camera();

// Get global camera
CAMERA* get_global_camera();

// Use global camera
void use_global_camera();

// Move the global camera
void move_camera(int hdir, int vdir);

// Update the camera
void update_camera(float tm);

#endif // __CAMERA__
