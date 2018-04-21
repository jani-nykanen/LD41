// LD41
// Configuration (source)
// (c) 2018 Jani Nykänen

#include "config.h"

#include "../lib/parseword.h"


// Read a config file
int config_read(CONFIG* c, const char* path) {

    // Open a config file
    PW_WORDDATA* w = pw_parse_file(path);
    if(w == NULL) {

        return 1;
    }

    // Key
    static char* key;
    // Value
    static char* value;

    // Read words
    int i = 1;
    for(; i < w->wordCount -1; ++ i) {

        if(strcmp(pw_get_word(w,i),"=") == 0) {

            key = pw_get_word(w,i-1);
            if(key[0] != '$') continue;

            value = pw_get_word(w,i+1);

            if(strcmp(key,"$window_caption") == 0) {

                strcpy(c->caption,value);
            }
            else if(strcmp(key,"$window_width") == 0) {

                c->winSize.x = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$window_height") == 0) {

                c->winSize.y = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$full_screen") == 0) {

                c->fullscreen = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$canvas_width") == 0) {

                c->canvasSize.x = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$canvas_height") == 0) {

                c->canvasSize.y = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$frame_rate") == 0) {

                c->frameRate = (int)strtol(value,NULL,10);
            }
            else if(strcmp(key,"$asset_path") == 0) {

                strcpy(c->assetPath,value);
            }

        }
    }

    // Free memory
    pw_destroy_word_data(w);

    return 0;
}