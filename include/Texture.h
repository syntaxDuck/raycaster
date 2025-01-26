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

Texture *create_textures();
Uint32 *load_image_data(const char *filePath, int *width, int *height);
void free_texture(Texture *texture);

#endif
