#ifndef UTILITY_H
#define UTILITY_H

#include <SDL2/SDL_render.h>
#include "Structs.h"

void rotateVector(Vector *vect, double angle);
void moveVector(Vector *vector, double x, double y);
void scaleVector(Vector *vector, double mag);
void rescaleVector(Vector *vector, double new_mag);
void calculateVectorMag(Vector *vect);
void setPoint(Point *point, double x, double y);
void setVector(Vector *vect, double x, double y, double mag, double angle);
Point translatePoints(Point p1, Point p2);
Vector transposeVector(Point origin, Vector vector);

void initSDL(SDL_Window **window, SDL_Renderer **renderer, int win_width,
             int win_height);

#endif
