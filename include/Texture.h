#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdint.h>
#include "Defines.h"

typedef struct
{
    int *pixels;
    int height;
    int width;
} Texture;

Texture *createTextures();
int *loadImageData(const char *filePath, int *width, int *height);
void freeTexture(Texture *texture);

#endif