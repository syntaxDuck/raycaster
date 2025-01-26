#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <dirent.h>
#include <stdint.h>

typedef struct {
  int *pixels;
  int height;
  int width;
} Texture;

Texture *createTextures();
Uint32 *loadImageData(const char *filePath, int *width, int *height);
void freeTexture(Texture *texture);

#endif
