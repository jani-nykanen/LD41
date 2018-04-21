// LD41
// Graphics routines (source)
// (c) 2018 Jani Nykänen

#include "graphics.h"

#include "error.h"
#include "mathext.h"

#include "../include/std.h"

// Used ("global") frame
static FRAME* gframe =NULL;
// Used frame
static FRAME* usedFrame =NULL;
// Global renderer
static SDL_Renderer* grend =NULL;
// Canvas texture
static SDL_Texture* texCanvas =NULL;

// Alpha color
static Uint8 alpha = 170;

// Translation
static POINT tr;


// Clip bitmap
static bool clip(BITMAP* bmp, int* dx, int* dy, int* sx, int* sy, int* sw, int* sh) {

    // Clip
    if(*dx + *sw >= gframe->width) {
        
        *sw = gframe->width - *dx;
    }
    else if(*dx <= 0) {
        
        *sx += -*dx;
        *sw += *dx;
        *dx = 0;
    }
    if(*dy + *sh >= gframe->height) {
        
        *sh = gframe->height - *dy;
    }
    else if(*dy < 0) {
        
        *sy += -*dy;
        *sh += *dy;
        *dy = 0;
    }

    // Check if we have anything to be drawn
    if(*sw <= 0 || *sh <= 0 || *sx >= bmp->width || *sy >= bmp->height)
        return false;

    return true;
}


// Initialize
void graphics_init(SDL_Renderer* rend) {

    grend = rend;
    tr = point(0, 0);
}


// Bind frame
void bind_frame(FRAME* f) {

    gframe = f;
    usedFrame = f;
}


// Clear screen
void clear(Uint8 color) {

    memset(gframe->data, color, gframe->width * gframe->height);
}


// Set alpha
void set_alpha(Uint8 color) {

    alpha = color;
}


// Get alpha
Uint8 get_alpha() {

    return alpha;
}


// Use frame to create a canvas texture
int create_canvas_texture(FRAME* f) {

    // Create canvas
    texCanvas = SDL_CreateTexture(grend, 
        SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING, 
        f->width, f->height);
    
    if(texCanvas == NULL) {

        error_throw("Failed to create a canvas texture!", NULL);
    }

    return 0;
}


// Draw canvas texture
void draw_canvas_texture(POINT pos, POINT size) {

    gframe = usedFrame;

    // Update texture
    SDL_UpdateTexture(texCanvas, NULL, gframe->data, gframe->width);

    // Draw canvas texture
    SDL_Rect dest = (SDL_Rect){pos.x,pos.y,size.x,size.y};
    SDL_RenderCopy(grend,texCanvas,NULL,&dest);
}


// Fill rectangle
void fill_rect(int x, int y, int w, int h, Uint8 color) {

    x += tr.x;
    y += tr.y;

    // Clip
    if(x < 0) {

        w += x;
        x = 0;
    }
    else if(x+w >= gframe->width) {

        w = gframe->width -x;
    }
    if(y < 0) {

        h += y;
        y = 0;
    }
    else if(y+h >= gframe->height) {

        h = gframe->height -y;
    }

    // Do we have anything to be drawn?
    if(w <= 0 || h <= 0)
        return;

    int offset = y * gframe->width + x;

    int i = 0;
    for(; i < h; ++ i) {

        memset(gframe->data + offset, color, w);
        offset += gframe->width;
    }
}


// Draw a bitmap
void draw_bitmap(BITMAP* bmp, int dx, int dy, int flip) {

    draw_bitmap_region(bmp,0,0,bmp->width,bmp->height,dx,dy,flip);
}


// Draw a bitmap region
void draw_bitmap_region(BITMAP* bmp, int sx, int sy, int sw, int sh, 
    int dx, int dy, int flip) {

    if(bmp == NULL) return;

    dx += tr.x;
    dy += tr.y;

    // Clip
    if(!clip(bmp,&dx,&dy,&sx,&sy,&sw,&sh))
        return;

    bool hflip = (flip & FLIP_H) != 0;
    bool vflip = (flip & FLIP_V) != 0;

    int offset = dy * gframe->width + dx;
    int pixel = sy * bmp->width + sx; 
    int pjump = bmp->width - sw;

    if(vflip) {

        offset += (sh-1) * gframe->width;
    }

    if(hflip) {

        offset += sw;
    }

    Uint8 col;
    int x,y;
    for(y=0; y < sh; ++ y) {

        for(x=0; x <  sw; ++ x) {
        
            if(pixel >= bmp->width*bmp->height)
                return;

            col = bmp->data[pixel ++];

            if(col != alpha && offset >= 0)
                gframe->data[offset] = col;

            if(hflip)
                -- offset;
            else
                ++ offset;
        }
        pixel += pjump;

        if(hflip) {

            offset += sw * 2;
        }

        offset -= sw;

        if(vflip) {

            offset -= gframe->width; 
        }
        else {

            offset += gframe->width; 
        }
        
        
    }
}


// Faster rendering routine (no alpha or flipping)
void draw_bitmap_fast(BITMAP* bmp, int dx, int dy) {

    draw_bitmap_region_fast(bmp,0,0,bmp->width, bmp->height, dx, dy);
}


// Faster routine for drawing a bitmap region (no alpha or flipping)
void draw_bitmap_region_fast(BITMAP* bmp, 
    int sx, int sy, int sw, int sh, int dx, int dy) {

    if(bmp == NULL) return;

    dx += tr.x;
    dy += tr.y;

    // Clip
    if(!clip(bmp,&dx,&dy,&sx,&sy,&sw,&sh))
        return;

    int offset = dy * gframe->width + dx;
    int pixel = sy * bmp->width + sx;

    int y = 0;
    for(; y < sh; ++ y) {

        memcpy(gframe->data + offset, bmp->data + pixel, sw);

        offset += gframe->width;
        pixel += bmp->width;
    }
}


// Draw text with a bitmap font
void draw_text(BITMAP* font, const char* text, 
    int dx, int dy, int xoff, int yoff, bool center) {

    int len = strlen((const char*)text);

    int x = dx;
    int y = dy;
    int cw = font->width / 16;
    int ch = cw;
    int i=0;
    unsigned char c;
    int sx;
    int sy;

    if(center) {

        dx -= (len* + cw) / 2;
        x = dx;
    }

    for(; i < len;  ++ i) {

        c = text[i];
        if(c == '\n') {

            x = dx;
            y += yoff + ch;
            continue;
        }

        sx = c % 16;
        sy = c / 16;

        draw_bitmap_region(font,sx*cw,sy*ch,cw,ch,x,y, FLIP_NONE);

        x += cw + xoff;
    }
}


// Draw top or bottom half of the triangle
static void draw_triangle_half(int midx, int midy, int endMid, int endy, 
    int stepx, int stepy, float dx, float dend, int x1, int y1, Uint8 color) {

    float sx = (float) midx;
    float ex = (float) endMid;

    int x, y;
    int offset;

    for(y = midy; (stepy > 0 ? y <= endy : y >= endy); y += stepy) {

        x = (int)sx;
        offset = y * gframe->width + x;
        for(; (stepx > 0 ? x <= (int)ex : x >= (int)ex); x += stepx) {

            if(x >= 0 && x < gframe->width && y >=0 && y < gframe->height) {
                
                gframe->data[offset] = color;

            }

            offset += stepx;
        }

        sx += dx;
        ex -= dend;
    }

}


// Draw a triangle
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, Uint8 color) {

    // TODO: Check if the points are NOT in the same line

    // Set points
    POINT min = point(x1, y1);
    POINT mid = point(x2, y2);
    POINT max = point(x3, y3);
    order_points_y_3(&min, &mid, &max);

    // If not in the screen, do not draw
    if(min.y >= gframe->height-1 || max.y < 0 ||
      min_3(x1,x2,x3) >= gframe->width-1 || max_3(x1,x2,x3) < 0) {

        return;
    }

    // Calculate x delta (per one step in y axis)
    float dx1, dx2, dend;
    if(mid.y != min.y)
        dx1 = - (float)(mid.x - min.x) / (float) (mid.y - min.y);
    else
        dx1 = 0;

    if(mid.y != max.y)
        dx2 =  (float)(mid.x - max.x) / (float) (mid.y - max.y);
    else 
        dx2 = 0;
    
    dend = (float)(max.x - min.x) / (float) (max.y - min.y);

    // "End mid" is the y coordinate in the "end line" (i.e
    // where every rendered line ends)
    int endMid = min.x + dend * (mid.y - min.y);
    
    // Set horizontal step
    int stepx = (int)mid.x < endMid ? 1 : -1;
    
    // Draw the upper half
    if(mid.y != min.y) {
        
        draw_triangle_half(mid.x,mid.y,endMid, max_2(min.y,-1),stepx, -1, dx1, dend,x1,y1, color);
    }

    // Draw the bottom half
    if(mid.y != max.y) {

        draw_triangle_half(mid.x,mid.y,endMid, min_2(max.y, gframe->height),stepx, 1, dx2, -dend,x1,y1, color);
    }
}


// Draw a line
void draw_line(int x1, int y1, int x2, int y2, Uint8 color) {

    // Bresenham's line algorithm
    int dx = abs(x2-x1), sx = x1<x2 ? 1 : -1;
    int dy = abs(y2-y1), sy = y1<y2 ? 1 : -1; 
    int err = (dx>dy ? dx : -dy)/2, e2;
     
    while(true) {

        if(!(y1 >= gframe->height-1 || y1 < 0 ||
           x1 >= gframe->width-1 || x1 < 0 ))
            gframe->data[y1 * gframe->width + x1] = color;
        
        if (x1==x2 && y1==y2) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x1 += sx; }
        if (e2 < dy) { err += dx; y1 += sy; }
    }
}


// Get the global frame
FRAME* get_global_frame() {

    return gframe;
}


// Translate
void translate(int x, int y) {

    tr.x = x;
    tr.y = y;
}


// Set the render target
void set_render_target(BITMAP* target) {

    if(target == NULL) {

        gframe = usedFrame;
    }
    else {

        usedFrame = gframe;
        gframe = target;
    }
}

