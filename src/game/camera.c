// LD41
// Camera (source)
// (c) 2018 Jani Nykänen

#include "camera.h"

#include "../include/std.h"

// Global camera
static CAMERA cam;


// Initialize global camera
void init_global_camera() {

    cam.pos = vec2(0,0);
}


// Get global camera
CAMERA* get_global_camera() {

    return &cam;
}


// Use global camera
void use_global_camera() {

    translate((int)floor(cam.pos.x), (int)floor(cam.pos.y));
}
