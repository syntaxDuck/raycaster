#include "Texture.h"

int *loadImageData(const char *filePath, int *width, int *height)
{
    // Load the image into an SDL_Surface
    SDL_Surface *surface = IMG_Load(filePath);
    if (!surface)
    {
        fprintf(stderr, "Could not load image: %s\n", IMG_GetError());
        return NULL;
    }

    // Store the width and height
    *width = surface->w;
    *height = surface->h;
    SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
    printf("Loaded surface: %dx%d, format: %d\n",
           surface->w, surface->h, surface->format->format);
    // Allocate an array of ints to hold the pixel data
    int *pixelData = malloc(surface->w * surface->h * sizeof(int));
    if (!pixelData)
    {
        fprintf(stderr, "Could not allocate memory for pixel data\n");
        SDL_FreeSurface(surface);
        return NULL;
    }

    // Copy the pixel data from the surface to the int array
    Uint32 *pixels = (Uint32 *)surface->pixels; // Cast pixels to Uint32*
    int pitch = surface->pitch / sizeof(Uint32);
    int pixel_num = 0;
    for (int y = 0; y < surface->h; y++)
    {
        for (int x = 0; x < surface->w; x++)
        {
            // Get the pixel value and store it in the array
            Uint32 pixel = pixels[y * surface->w + x];
            pixelData[y * surface->w + x] = pixel;
        }
    }

    // Clean up the surface
    SDL_FreeSurface(surface);

    return pixelData;
}

void createTextures()
{
    int width, height;

    // int *test = loadImageData("../assets/textures/pillar.png",
    //                           &width, &height);

    for (int x = 0; x < TEX_WIDTH; x++)
    {
        for (int y = 0; y < TEX_HEIGHT; y++)
        {
            int xorcolor = (x * 256 / TEX_WIDTH) ^ (y * 256 / TEX_HEIGHT);
            // int xcolor = x * 256 / TEX_WIDTH;
            int ycolor = y * 256 / TEX_HEIGHT;
            int xycolor = y * 128 / TEX_HEIGHT + x * 128 / TEX_WIDTH;
            // textures[0][TEX_WIDTH * y + x] = test[TEX_HEIGHT * y + x];
            textures[0][TEX_WIDTH * y + x] = 65536 * 254 * (x != y && x != TEX_WIDTH - y); // flat red texture with black cross
            textures[1][TEX_WIDTH * y + x] = xycolor + 256 * xycolor + 65536 * xycolor;    // sloped greyscale
            textures[2][TEX_WIDTH * y + x] = 256 * xycolor + 65536 * xycolor;              // sloped yellow gradient
            textures[3][TEX_WIDTH * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; // xor greyscale
            textures[4][TEX_WIDTH * y + x] = 256 * xorcolor;                               // xor green
            textures[5][TEX_WIDTH * y + x] = 65536 * 192 * (x % 16 && y % 16);             // red bricks
            textures[6][TEX_WIDTH * y + x] = 65536 * ycolor;                               // red gradient
            textures[7][TEX_WIDTH * y + x] = 128 + 256 * 128 + 65536 * 128;                // flat grey texture
        }
    }
}