#ifndef UTILITY_H
#define UTILITY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdlib.h>

#include "vector.h"

Vector *generateCirclePoints(Vector center, double radius, int numPoints);
void generateFilledCircle(SDL_Renderer *renderer, Vector center, double radius,
                          int numPoints);

SDL_Texture *drawFilledCircle(SDL_Renderer *renderer, Vector center_vect,
                              int radius);
#endif
