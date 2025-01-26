#include "Texture.h"
#include "config.h"

Uint32 *loadImageData(const char *filePath, int *width, int *height) {
  // Load the image into an SDL_Surface
  SDL_Surface *surface = IMG_Load(filePath);
  if (!surface) {
    fprintf(stderr, "Could not load image: %s\n", IMG_GetError());
    return NULL;
  }

  // Store the width and height
  *width = surface->w;
  *height = surface->h;

  // Convert surface to RGBA8888 format
  SDL_Surface *converted_surface =
      SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
  SDL_FreeSurface(surface); // Free the original surface
  if (!converted_surface) {
    fprintf(stderr, "Could not convert surface to RGBA8888: %s\n",
            SDL_GetError());
    return NULL;
  }

  printf("Loaded surface: %dx%d, format: %d\n", converted_surface->w,
         converted_surface->h, converted_surface->format->format);

  // Allocate an array to hold the pixel data
  Uint32 *pixelData =
      malloc(converted_surface->w * converted_surface->h * sizeof(Uint32));
  if (!pixelData) {
    fprintf(stderr, "Could not allocate memory for pixel data\n");
    SDL_FreeSurface(converted_surface);
    return NULL;
  }

  // Copy the pixel data from the surface to the array
  Uint32 *pixels = (Uint32 *)converted_surface->pixels;
  for (int y = 0; y < converted_surface->h; y++) {
    for (int x = 0; x < converted_surface->w; x++) {
      // Get the pixel value and store it in the array
      Uint32 pixel = pixels[y * converted_surface->w + x];
      pixelData[y * converted_surface->w + x] = pixel;
    }
  }

  // Clean up the converted surface
  SDL_FreeSurface(converted_surface);

  return pixelData;
}

// TODO: have some issues rendering textures with alpha channels
Texture *createTextures() {
  // int textures[8][TEX_HEIGHT * TEX_WIDTH];
  Texture *textures = malloc(sizeof(Texture) * 11);
  int width, height;

  // Static Textures
  Uint32 *t0 =
      loadImageData("../assets/textures/bluestone.png", &width, &height);
  Uint32 *t1 =
      loadImageData("../assets/textures/colorstone.png", &width, &height);
  Uint32 *t2 = loadImageData("../assets/textures/eagle.png", &width, &height);
  Uint32 *t3 =
      loadImageData("../assets/textures/greystone.png", &width, &height);
  Uint32 *t4 = loadImageData("../assets/textures/mossy.png", &width, &height);
  Uint32 *t5 =
      loadImageData("../assets/textures/purplestone.png", &width, &height);
  Uint32 *t6 =
      loadImageData("../assets/textures/redbrick.png", &width, &height);
  Uint32 *t7 = loadImageData("../assets/textures/wood.png", &width, &height);

  // Sprit textures
  Uint32 *t8 = loadImageData("../assets/textures/barrel.png", &width, &height);
  Uint32 *t9 = loadImageData("../assets/textures/pillar.png", &width, &height);
  Uint32 *t10 =
      loadImageData("../assets/textures/greenlight.png", &width, &height);

  textures[0].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[1].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[2].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[3].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[4].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[5].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[6].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[7].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[8].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[9].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[10].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));

  for (int x = 0; x < TEX_WIDTH; x++) {
    for (int y = 0; y < TEX_HEIGHT; y++) {
      textures[0].pixels[TEX_WIDTH * y + x] = t0[TEX_HEIGHT * y + x];
      textures[1].pixels[TEX_WIDTH * y + x] = t1[TEX_HEIGHT * y + x];
      textures[2].pixels[TEX_WIDTH * y + x] = t2[TEX_HEIGHT * y + x];
      textures[3].pixels[TEX_WIDTH * y + x] = t3[TEX_HEIGHT * y + x];
      textures[4].pixels[TEX_WIDTH * y + x] = t4[TEX_HEIGHT * y + x];
      textures[5].pixels[TEX_WIDTH * y + x] = t5[TEX_HEIGHT * y + x];
      textures[6].pixels[TEX_WIDTH * y + x] = t6[TEX_HEIGHT * y + x];
      textures[7].pixels[TEX_WIDTH * y + x] = t7[TEX_HEIGHT * y + x];
      textures[8].pixels[TEX_WIDTH * y + x] = t8[TEX_HEIGHT * y + x];
      textures[9].pixels[TEX_WIDTH * y + x] = t9[TEX_HEIGHT * y + x];
      textures[10].pixels[TEX_WIDTH * y + x] = t10[TEX_HEIGHT * y + x];
    }
  }

  return textures;
}

Texture *createProgrammaticTextures() {
  Texture *textures = malloc(sizeof(Texture) * 11);
  textures[0].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[1].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[2].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[3].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[4].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[5].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[6].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[7].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[8].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[9].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  textures[10].pixels = malloc(sizeof(int) * (TEX_WIDTH * TEX_HEIGHT));
  for (int x = 0; x < TEX_WIDTH; x++) {
    for (int y = 0; y < TEX_HEIGHT; y++) {
      int xorcolor = (x * 256 / TEX_WIDTH) ^ (y * 256 / TEX_HEIGHT);
      int xcolor = x * 256 / TEX_WIDTH;
      int ycolor = y * 256 / TEX_HEIGHT;
      int xycolor = y * 128 / TEX_HEIGHT + x * 128 / TEX_WIDTH;
      textures[0].pixels[TEX_WIDTH * y + x] =
          65536 * 254 *
          (x != y && x != TEX_WIDTH - y); // flat red pixels with black cross
      textures[1].pixels[TEX_WIDTH * y + x] =
          xycolor + 256 * xycolor + 65536 * xycolor; // sloped greyscale
      textures[2].pixels[TEX_WIDTH * y + x] =
          256 * xycolor + 65536 * xycolor; // sloped yellow gradient
      textures[3].pixels[TEX_WIDTH * y + x] =
          xorcolor + 256 * xorcolor + 65536 * xorcolor;       // xor greyscale
      textures[4].pixels[TEX_WIDTH * y + x] = 256 * xorcolor; // xor green
      textures[5].pixels[TEX_WIDTH * y + x] =
          65536 * 192 * (x % 16 && y % 16);                   // red bricks
      textures[6].pixels[TEX_WIDTH * y + x] = 65536 * ycolor; // red gradient
      textures[7].pixels[TEX_WIDTH * y + x] =
          128 + 256 * 128 + 65536 * 128; // flat grey pixels
    }
  }
  return textures;
}

void freeTexture(Texture *textures) {
  for (int i = 0; i < 11; i++) {
    free(textures[i].pixels);
  }
  free(textures);
}
