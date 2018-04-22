// Assets (source)
// (c) 2018 Jani Nykänen

#include "assets.h"

#include "bitmap.h"
#include "error.h"

#include "../lib/tmxc.h"
#include "../lib/parseword.h"

#include "../include/std.h"

// Asset type enum
enum {

    T_BITMAP = 0,
    T_TILEMAP = 1,
};


// Get asset type
static int get_asset_type(const char* value) {

    if(strcmp(value,"bitmap")  == 0) {

        return T_BITMAP;
    }
    else if(strcmp(value,"tilemap") == 0) {

        return T_TILEMAP;
    }
    

    return -1;
}


// Load an asset
static void* load_asset(const char* path, int type) {

    switch(type) {

    case T_BITMAP:
        return bitmap_load(path);

    case T_TILEMAP:
        return load_tilemap(path);

    default:
        return NULL;
    }
}


// Load an asset pack
ASSET_PACK* load_asset_pack(const char* fpath) {

    // Open a file
    PW_WORDDATA* w = pw_parse_file(fpath);
    if(w == NULL) {

        return NULL;
    }

    // Allocate memory
    ASSET_PACK* p = malloc(sizeof(ASSET_PACK));
    if(p == NULL) {

        error_mem_alloc();
        return NULL;
    }
    p->assetCount = 0;

    char* filePath;
    int assetType;       

    int i = 0;
    int count = 0;
    char* word;
    char* param;
    char* value;

    char* name;
    char path[NAME_STRING_SIZE];

    // Go through words
    for(; i < w->wordCount; ++ i) {

        word = pw_get_word(w, i);
        if(i > 0 && strcmp(word, "=") == 0) {
             
            param = pw_get_word(w, i-1);
            value = pw_get_word(w, i+1);

            // Asset type
            if(strcmp(param,"$type") == 0) {

                assetType = get_asset_type(value);
            }
            // Asset path
            else if(strcmp(param,"$path") == 0) {

                filePath = value;
            }

            ++ i;
        }
        else if(word[0] != '$') {

            if(count == 0) {

                name = word;
            }
            else {

                snprintf(path, NAME_STRING_SIZE, "%s%s", filePath, word);
                p->objects[p->assetCount] = load_asset(path,assetType);
                if(p->objects[p->assetCount] == NULL) {

                    return NULL;
                }
                strcpy(p->names[p->assetCount].data, name);
                ++ p->assetCount;
            }

            count = !count;
        }
    }

    return p;
}


// Get asset pack
ANY assets_get(ASSET_PACK* p, const char* name) {

    int i = 0;
    // Go through names and try to find an asset with the given name
    for(; i < p->assetCount; ++ i)
    {
        if(strcmp(name,p->names[i].data) == 0)
        {
            return p->objects[i];
        }
    }

    return NULL;
}


// Destroy an asset pack
void assets_destroy(ASSET_PACK* p) {

    int i = 0;
    ANY obj;
    for(; i < p->assetCount; ++ i) {   

        obj = p->objects[i];
        switch(p->types[i])
        {
        case T_BITMAP:
            bitmap_destroy((BITMAP*)obj);
            break;
        case T_TILEMAP:
            destroy_tilemap((TILEMAP*)obj);
            break;

        default:
            break;
        }
    }
}
