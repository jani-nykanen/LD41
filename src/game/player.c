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
    if(input_get_cursor_pos().x < 248 && 
        input_get_mouse_button(1) == STATE_PRESSED) {

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

    const float PSPEED = 16.0f;

    pl->flip = pl->moving ? (pl->tpos.x < pl->pos.x ? 1 : 0) : pl->flip;

    if(pl->moving) {

        spr_animate(&pl->tspr,0,0,3, 8, tm);

        if(pl->pspeed < PSPEED) {

            pl->pspeed += 0.5f * tm;
            if(pl->pspeed > PSPEED) {

                pl->pspeed = PSPEED;
            }
        }
    }
    else {

        if(pl->pspeed > 0.0f) {

            pl->pspeed -= 0.5f * tm;
            if(pl->pspeed < 0.0f)
                pl->pspeed = 0.0f;
        }
    }

    if(pl->pspeed > 0.0f)
        spr_animate(&pl->pspr,2,0,3, 18 - pl->pspeed, tm);
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
    pl.spr.row = 1;
    pl.tspr = create_sprite(16,16);
    pl.pspr = create_sprite(16,16);
    pl.pspr.row = 2;
    pl.flip = FLIP_NONE;
    pl.pspeed = 0.0f;

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

    if(pl->moving) {

        spr_draw(&pl->tspr, bmpPlayer, (int)round(pl->tpos.x)-8, (int)round(pl->tpos.y)-8, 0);
    }

    spr_draw(&pl->spr, bmpPlayer, (int)round(pl->pos.x)-8, (int)round(pl->pos.y)-8, pl->flip);
    spr_draw(&pl->pspr, bmpPlayer, (int)round(pl->pos.x)-8, (int)round(pl->pos.y)-8-16, pl->flip);
}


// Surface collision
bool pl_surface_collision(PLAYER* pl, int type, int x, int y, int d, float tm) {

    VEC2 p = pl->pos;
    const int DIM = 6;

    switch(type) {

    case COL_DOWN:
    {

        if(p.x+DIM > x && p.x-DIM < x+d 
        && pl->speed.y > 0.0f && p.y+DIM > y-1*tm && p.y+DIM < y+8+pl->speed.y*tm) {

            pl->pos.y = y - DIM;
            pl->speed.y = 0.0f;

            return true;
        }
    }
    break;

    case COL_UP:
    {

        if(p.x+DIM > x && p.x-DIM < x+d 
        && pl->speed.y < 0.0f && p.y-DIM < y+tm && p.y-DIM > y-8+pl->speed.y*tm) {

            pl->pos.y = y + DIM;
            pl->speed.y = 0.0f;

            return true;
        }
    }
    break;
    
    case COL_RIGHT:
    {

        if(p.y+DIM > y && p.y-DIM < y+d 
        && pl->speed.x > 0.0f && p.x+DIM > x-tm && p.x+DIM < x+8+pl->speed.x*tm) {

            pl->pos.x = x - DIM;
            pl->speed.x = 0.0f;

            return true;
        }
    }
    break;

    case COL_LEFT:
    {

        if(p.y+DIM > y && p.y-DIM < y+d 
        && pl->speed.x < 0.0f && p.x-DIM < x+tm && p.x-DIM > x-8+pl->speed.x*tm) {

            pl->pos.x = x + DIM;
            pl->speed.x = 0.0f;

            return true;
        }
    }
    break;

    default:
        break;
    }

    return false;

}