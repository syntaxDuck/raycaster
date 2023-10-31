#ifndef UTILITY_H
#define UTILITY_H

#include <SDL2/SDL_render.h>

typedef struct {
  double x;
  double y;
  double z;
} Vector;

typedef struct {
  Vector origin;
  Vector offset;
} Rel_Vector;

void rotateVector(int cx, int cy, int angle, Vector *vect);

void generateFilledCircle(SDL_Renderer *renderer, Vector center, double radius,
                          int numPoints);

SDL_Texture *drawFilledCircle(SDL_Renderer *renderer, Vector center_vect,
                              int radius);

Vector *generateCirclePoints(Vector center, double radius, int numPoints);

#endif