#ifndef UTILITY_H
#define UTILITY_H

#include <SDL2/SDL_render.h>

typedef struct {
  double x;
  double y;
  double z;
} Point;

typedef struct {
  Point point;

  double mag;
  double angle;
} Vector;

void rotateVector(Vector *vect, double angle);
void moveVector(Vector *vector, double x, double y);
void scaleVector(Vector *vector, double mag);
void rescaleVector(Vector *vector, double new_mag);
Point translatePoints(Point p1, Point p2);
Vector transposeVector(Point origin, Vector vector);

void generateFilledCircle(SDL_Renderer *renderer, Vector center, double radius,
                          int numPoints);

SDL_Texture *drawFilledCircle(SDL_Renderer *renderer, Vector center_vect,
                              int radius);

Vector *generateCirclePoints(Vector center, double radius, int numPoints);

void initSDL(SDL_Window **window, SDL_Renderer **renderer, int win_width,
             int win_height);

#endif
