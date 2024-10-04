#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdint.h>
#include "Defines.h"

void createTextures();
int *loadImageData(const char *filePath, int *width, int *height);

#endif