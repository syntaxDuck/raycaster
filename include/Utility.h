#ifndef UTILITY_H
#define UTILITY_H

#include <SDL.h>
#include <SDL_render.h>
#include <stdlib.h>

#include "vector.h"

Vector *generate_circle_points(Vector center, double radius, int numPoints);
void generate_filled_circle(SDL_Renderer *renderer, Vector center,
                            double radius, int numPoints);

SDL_Texture *draw_filled_circle(SDL_Renderer *renderer, Vector center_vect,
                                int radius);
#endif
