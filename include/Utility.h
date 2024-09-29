#ifndef UTILITY_H
#define UTILITY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdlib.h>

#include "Structs.h"

void rotateVector(Vector *vect, double angle);
void scaleVector(Vector *vector, double mag);
void rescaleVector(Vector *vector, double new_mag);
void calculateVectorMag(Vector *vect);

Vector setVector(double x, double y, double mag, double angle);
Vector transposeVector(Vector origin, Vector vector);
Vector normalizeVector(Vector vector);

#endif
