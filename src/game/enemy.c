// LD41
// Enemy (source)
// (c) 2018 Jani Nykäen

#include "enemy.h"

#include "camera.h"

#include "../global.h"

#include "../include/system.h"
#include "../engine/mathext.h"


// Create an enemy
ENEMY create_enemy(VEC2 pos) {

    ENEMY e;
    e.pos = pos;
    e.radius = 12.0f + (float)(rand() % 1000)/1000.0f * 20.0f;
    e.angle = (float)(rand() % 1000) / 1000.0f * 2 * M_PI;
    e.triAngle = e.angle;
    e.active = false;
    e.exist = true;
    e.size = 0.0f;
    e.dead = false;
    float speedMod = 1.5f - e.radius/32.0f;

    e.speed.x = cos(e.angle) * speedMod ;
    e.speed.y = sin(e.angle) * speedMod;

    e.grid.x = (int)floor(e.pos.x / 224.0f);
    e.grid.y = (int)floor(e.pos.y / 176.0f);

    return e;
}


// Update an enemy
void enemy_update(ENEMY* e, PLAYER* pl, float tm) {

    if(!e->exist) return;
    if(get_global_camera()->moving) return;

    VEC2 cpos = get_global_camera()->pos;


    e->active = !(e->pos.x+e->radius < cpos.x || e->pos.x-e->radius > cpos.x+224 
        || e->pos.y+e->radius < cpos.y || e->pos.y-e->radius > cpos.y+176);

    if(!e->active) {

        e->size = 0.0f;
        return;
    }

    // Points
    int cx = (int)e->pos.x;
    int cy = (int)e->pos.y;

    e->x1 = cx + (int) (cos(e->triAngle) * e->radius * e->size);
    e->y1 = cy + (int) (sin(e->triAngle) * e->radius * e->size);
    e->x2 = cx + (int) (cos(e->triAngle + M_PI*2.0f / 3.0f) * e->radius * e->size);
    e->y2 = cy + (int) (sin(e->triAngle + M_PI*2.0f / 3.0f) * e->radius * e->size);
    e->x3 = cx + (int) (cos(e->triAngle + M_PI*4.0f / 3.0f) * e->radius * e->size);
    e->y3 = cy + (int) (sin(e->triAngle + M_PI*4.0f / 3.0f) * e->radius * e->size);

    // Dying
    if(e->dead) {

        e->size -= 0.05f * tm;
        if(e->size <= 0.0f)
            e->exist = false;

        return;
    }

    e->triAngle += 0.05f * tm * (e->speed.x >= 0.0f ? 1 : -1);

    // Grow
    if(e->size < 1.0f) {

        e->size += 0.025f * tm;
        if(e->size > 1.0f)
            e->size = 1.0f;
    }
    else {
        // Move
        e->pos.x += e->speed.x * tm;
        e->pos.y += e->speed.y * tm;
    }

    // Collisions
    if(e->speed.x < 0.0f && e->pos.x-e->radius <= e->grid.x*224) {

        e->pos.x = e->grid.x*224 + e->radius;
        e->speed.x *= -1.0f;
    }
    else if(e->speed.x > 0.0f && e->pos.x+e->radius >= (e->grid.x+1)*224) {

        e->pos.x = (e->grid.x+1)*224 - e->radius;
        e->speed.x *= -1.0f;
    }

    if(e->speed.y < 0.0f && e->pos.y-e->radius <= e->grid.y*176) {

        e->pos.y = e->grid.y*176 + e->radius;
        e->speed.y *= -1.0f;
    }
    else if(e->speed.y > 0.0f && e->pos.y+e->radius >= (e->grid.y+1)*176) {

        e->pos.y = (e->grid.y+1)*176 - e->radius;
        e->speed.y *= -1.0f;
    }

    // If cursor overlays
    POINT c = input_get_cursor_pos();
    int tx = cpos.x - 12 + c.x;
    int ty = cpos.y - 12 + c.y;
    if(inside_triangle(tx,ty, e->x1,e->y1,e->x2,e->y2,e->x3,e->y3)) {

        set_cursor_mode(5);
        if(input_get_mouse_button(3) == STATE_PRESSED) {

            e->dead = true;
        }
    }

}


// Draw an enemy
void enemy_draw(ENEMY* e) {

    if(!e->exist || !e->active) return;

    draw_inverse_triangle(e->x1,e->y1,e->x2,e->y2,e->x3,e->y3);

}
