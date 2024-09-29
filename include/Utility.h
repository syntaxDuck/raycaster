#ifndef UTILITY_H
#define UTILITY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdlib.h>

#include "Structs.h"

void printVector(Vector vector);
void rotateVector(Vector *vect, double angle);
void scaleVector(Vector *vector, double mag);
void rescaleVector(Vector *vector, double new_mag);
void calculateVectorMag(Vector *vector);

Vector setVector(double x, double y);
Vector transposeVector(Vector origin, Vector vector);
Vector normalizeVector(Vector vector);

#endif
