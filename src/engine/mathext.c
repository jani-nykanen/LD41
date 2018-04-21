// LD41
// Math extensions (source)
// (c) 2018 Jani Nykänen

#include "mathext.h"


// Minimum (out of 3)
int min_3(int a, int b, int c) {

    return (a <= b && a <= c) ? a 
        : ( (b <= a && b <=c) ? b : c);
}


// Maximum (out of 3)
int max_3(int a, int b, int c) {

    return (a >= b && a >= c) ? a 
        : ( (b >= a && b >=c) ? b : c);
}


// Minimum (out of 3), float
float min_3f(float a, float b, float c) {

    return (a <= b && a <= c) ? a 
        : ( (b <= a && b <=c) ? b : c);
}


// Maximum (out of 3), float
float max_3f(float a, float b, float c) {

    return (a >= b && a >= c) ? a 
        : ( (b >= a && b >=c) ? b : c);
}


// Minimum (out of 2)
int min_2(int a, int b) {

    return (a <= b ? a : b);
}


// Maximum (out of 2)
int max_2(int a, int b) {

    return (a >= b ? a : b);
}



// Minimum (out of 2), float
float min_2f(float a, float b) {

    return (a <= b ? a : b);
}


// Maximum (out of 2), float
float max_2f(float a, float b) {

    return (a >= b ? a : b);
}


// Order three points, compare y axis
void order_points_y_3(POINT* a, POINT* b, POINT* c) {

    POINT p[3];
    p[0] = *a;
    p[1] = *b;
    p[2] = *c;
    int i = 0;

    POINT buf;
    bool changed = false;
    do {

        changed = false;
        for(i=1; i < 3; ++ i) {

            if(p[i].y < p[i-1].y) {

                buf = p[i];
                p[i] = p[i-1];
                p[i-1] = buf;
                changed = true;
                break;
            }
            
        }

    }
    while(changed);

    *a = p[0];
    *b = p[1];
    *c = p[2];
    
}