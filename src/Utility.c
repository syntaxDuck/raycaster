#include <SDL2/SDL.h>
#include <math.h>
#include <stdlib.h>

#include "Utility.h"

void rotateVector(int cx, int cy, int angle, Vector *vect) {
  double rad = angle * M_PI / 180;
  int newX = cx + (vect->x - cx) * cos(rad) - (vect->y - cy) * sin(rad);
  int newY = cy + (vect->x - cx) * sin(rad) + (vect->y - cy) * cos(rad);
  vect->x = newX;
  vect->y = newY;
}

// Function to generate points along the circumference of a circle using Bézier
// curves
Vector *generateCirclePoints(Vector center, double radius, int numPoints) {

  Vector *points = malloc(numPoints * sizeof(Vector));
  double step = 2 * M_PI / numPoints;
  for (int i = 0; i < numPoints; i++) {
    double t = i * step;
    points[i].x = center.x + radius * cos(t);
    points[i].y = center.y + radius * sin(t);
  }

  return points;
}

void generateFilledCircle(SDL_Renderer *renderer, Vector center, double radius,
                          int numPoints) {
  Vector *points;
  for (double r = radius; r > 0; r = r - 1) {
    SDL_SetRenderDrawColor(renderer, 0, 255 - r * 10, 0, 255);
    points = generateCirclePoints(center, r, numPoints);
    for (int x = 0; x < numPoints; x++) {
      SDL_RenderDrawPoint(renderer, points[x].x, points[x].y);
    }
    free(points);
  }
}

SDL_Texture *drawFilledCircle(SDL_Renderer *renderer, Vector center_vect,
                              int radius) {

  int diameter = 2 * radius;

  SDL_Surface *surface =
      SDL_CreateRGBSurface(0, diameter, diameter, 32, 0, 0, 0, 0);

  SDL_LockSurface(surface);

  Uint32 *pixels = (Uint32 *)surface->pixels;
  for (int x = center_vect.x - radius; x <= center_vect.x + radius; x++) {
    for (int y = center_vect.y - radius; y <= center_vect.y + radius; y++) {
      int dx = x - center_vect.x;
      int dy = y - center_vect.y;
      if (dx * dx + dy * dy <= radius * radius) {
        int index = (dy + radius) * diameter + (dx + radius);
        pixels[index] = SDL_MapRGB(surface->format, 255, 0, 0);
      }
    }
  }

  SDL_UnlockSurface(surface);

  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  return texture;
}

void initSDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("Error initializing SDL: %s\n", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
}

SDL_Window *initSDLWindow(int screen_width, int screen_height) {
  SDL_Window *window =
      SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       screen_height, screen_height, 0);
  if (window == NULL) {
    printf("Error creating SDL window: %s\n", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  return window;
}

SDL_Renderer *initSDLRenderer(SDL_Window *window) {
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    printf("Error creating SLD renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  return renderer;
}
