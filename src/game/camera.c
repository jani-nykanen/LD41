// LD41
// Camera (source)
// (c) 2018 Jani Nykänen

#include "camera.h"

#include "../include/std.h"

// Global camera
static CAMERA cam;


// Initialize global camera
void init_global_camera() {

    cam.grid = point(2,2);
    cam.target = cam.grid;
    cam.pos = vec2(cam.grid.x*224, cam.grid.y * 176);
}


// Get global camera
CAMERA* get_global_camera() {

    return &cam;
}


// Use global camera
void use_global_camera() {

    translate(-(int)floor(cam.pos.x), -(int)floor(cam.pos.y));
}


// Move the global camera
void move_camera(int hdir, int vdir) {

    if(cam.moving) return;

    cam.target.x += hdir;
    cam.target.y += vdir;
    cam.moving = true;
    cam.step = 0;
}


// Update the camera
void update_camera(float tm) {

    const float SPEED = 8.0f;

    if(!cam.moving) return;

    int hdir = 0;
    if(cam.grid.x != cam.target.x)
        hdir = cam.grid.x > cam.target.x ? -1 : 1;

    int vdir = 0;
    if(cam.grid.y != cam.target.y)
        vdir = cam.grid.y > cam.target.y ? -1 : 1;

    cam.pos.x += SPEED * hdir;
    cam.pos.y += SPEED * vdir;

    int maxStep = 0;
    if(hdir != 0) {

        maxStep = (int) ( 224.0f / SPEED);
    }
    else if(vdir != 0) {

        maxStep = (int) ( 176.0f / SPEED);
    }

    if(++ cam.step >= maxStep) {

        cam.moving = false;
        cam.grid = cam.target;
    }

}
