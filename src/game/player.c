// LD41
// Player (header)
// (c) 2018 Jani Nykänen

#include "player.h"

#include "camera.h"

#include "../global.h"

// Constants
static const float PL_SPEED = 0.05f;
static const float PL_TARGET = 1.5f;

// Bitmaps
static BITMAP* bmpPlayer;


// Control
static void pl_control(PLAYER* pl) {

    // Get cursor position in camera coords
    POINT cpos = input_get_cursor_pos();
    cpos.x -= 12;
    cpos.y -= 12;
    CAMERA* c = get_global_camera();
    cpos.x += (int)c->pos.x;
    cpos.y += (int)c->pos.y;

    // If special mode
    int m = get_cursor_mode();
    if(m != 0) {

        if(m == 1)
            cpos.y -= 16;

        else if(m == 2)
            cpos.x += 16;

        else if(m == 3)
            cpos.y += 16;

        else if(m == 4)
            cpos.x -= 16;
    }

    float angle;


    // Mouse button pressed
    if(input_get_mouse_button(1) == STATE_PRESSED) {

        pl->tpos = vec2((float)cpos.x, (float)cpos.y);
        pl->moving = true;
    }

    if(!pl->moving) {
        
        pl->target.x = 0.0f;
        pl->target.y = 0.0f;
    }
    else {

        // Get target speed
        angle = atan2(pl->tpos.y-pl->pos.y, pl->tpos.x-pl->pos.x);

        pl->target.x = cos(angle) * PL_TARGET;
        pl->target.y = sin(angle) * PL_TARGET;
    }
}


// Move axis
static void move_axis(float *pos, float* coord,  float* target, float speed, float tm) {

    if(*target > *coord) {

        *coord += speed * tm;
        if(*coord >= *target) {

            *coord = *target;
        }
    }
    else if(*target < *coord) {

        *coord -= speed * tm;
        if(*coord  <= *target) {

            *coord = *target;
        }
    }

    *pos += *coord *tm;
}



// Move
static void pl_move(PLAYER* pl, float tm) {

    const float DELTA = 4.0f;

    move_axis(&pl->pos.x,&pl->speed.x,&pl->target.x, PL_SPEED, tm);
    move_axis(&pl->pos.y,&pl->speed.y,&pl->target.y, PL_SPEED, tm);

    // Get distance
    float dist = hypot(pl->tpos.x-pl->pos.x, pl->tpos.y-pl->pos.y);
    if(dist < DELTA) {

        pl->moving = false;
    }
}


// Animate
static void pl_animate(PLAYER* pl, float tm) {

    pl->flip = pl->moving ? (pl->tpos.x < pl->pos.x ? 1 : 0) : pl->flip;
}


// Screen transition
static void pl_screen_transition(PLAYER* pl) {

    CAMERA* c = get_global_camera();

    if(pl->speed.x < 0.0f && pl->pos.x < c->pos.x) {

        move_camera(-1, 0);
    }
    else if(pl->speed.x > 0.0f && pl->pos.x > c->pos.x+224) {

        move_camera(1, 0);
    }
    else if(pl->speed.y < 0.0f && pl->pos.y < c->pos.y) {

        move_camera(0, -1);
    }
    else if(pl->speed.y > 0.0f && pl->pos.y > c->pos.y+176) {

        move_camera(0, 1);
    }

    if(c->moving) {

        pl->speed.x = 0;
        pl->speed.y = 0;
        pl->moving = false;
    }

}


// Create a player object
PLAYER create_player(VEC2 pos, ASSET_PACK* ass) {

    PLAYER pl;
    pl.pos = pos;
    pl.speed = vec2(0,0);
    pl.target = vec2(0,0);
    pl.tpos = pl.pos;
    pl.moving = false;
    pl.spr = create_sprite(16, 16);
    pl.flip = FLIP_NONE;

    if(ass != NULL) {

        bmpPlayer = (BITMAP*)assets_get(ass, "player");
    }

    return pl;
}


// Update player
void pl_update(PLAYER* pl, float tm) {

    pl_control(pl);
    pl_move(pl, tm);
    pl_animate(pl, tm);
    pl_screen_transition(pl);
}


// Draw player
void pl_draw(PLAYER* pl) {

    spr_draw(&pl->spr, bmpPlayer, (int)round(pl->pos.x)-8, (int)round(pl->pos.y)-8, pl->flip);
}
