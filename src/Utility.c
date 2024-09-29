#include "Utility.h"

void printVector(Vector vector)
{
  printf("X: %f, Y: %f, Mag: %f, Angle: %f\n", vector.x, vector.y, vector.mag, vector.angle);
}

Vector normalizeVector(Vector vector)
{
  Vector norm_vect;
  norm_vect.x /= vector.mag;
  norm_vect.y /= vector.mag;
  norm_vect.mag = 1;
  norm_vect.angle = vector.angle;
  return norm_vect;
}

void rotateVector(Vector *vector, double angle)
{
  double newX = vector->x * cos(angle) - vector->y * sin(angle);
  double newY = vector->x * sin(angle) + vector->y * cos(angle);
  vector->x = newX;
  vector->y = newY;

  vector->angle += angle;
  if (vector->angle < 0)
  {
    vector->angle += 2 * M_PI;
  }

  if (vector->angle >= 2 * M_PI)
  {
    vector->angle -= 2 * M_PI;
  }
}

void scaleVector(Vector *vector, double mag)
{
  vector->mag += mag;
  vector->x = vector->mag * cos(vector->angle);
  vector->y = vector->mag * sin(vector->angle);
}

void rescaleVector(Vector *vector, double new_mag)
{
  vector->mag = new_mag;
  vector->x = new_mag * cos(vector->angle);
  vector->y = new_mag * sin(vector->angle);
}

Vector transposeVector(Vector origin, Vector vector)
{
  Vector newVect;
  newVect.x = origin.x + vector.x;
  newVect.y = origin.y + vector.y;
  newVect.mag = vector.mag;
  newVect.angle = vector.angle;

  return newVect;
}

void calculateVectorMag(Vector *vector)
{
  vector->mag = sqrt(vector->x * vector->x + vector->y * vector->y);
}

void calculateVectorAngle(Vector *vector)
{
  vector->angle = atan2(vector->y, vector->x);
  if (vector->angle < 0)
  {
    vector->angle += 2 * M_PI; // Ensure the angle is between 0 and 2π
  }
}

Vector setVector(double x, double y)
{
  Vector vect;
  vect.x = x;
  vect.y = y;
  calculateVectorAngle(&vect);
  calculateVectorMag(&vect);
  return vect;
}

// Function to generate points along the circumference of a circle using Bézier
// curves
// Vector *generateCirclePoints(Vector center, double radius, int numPoints) {
//
//   Vector *points = malloc(numPoints * sizeof(Vector));
//   double step = 2 * M_PI / numPoints;
//   for (int i = 0; i < numPoints; i++) {
//     double t = i * step;
//     points[i].x = center.x + radius * cos(t);
//     points[i].y = center.y + radius * sin(t);
//   }
//
//   return points;
// }

// void generateFilledCircle(SDL_Renderer *renderer, Vector center, double
// radius,
//                           int numPoints) {
//   Vector *points;
//   for (double r = radius; r > 0; r = r - 1) {
//     SDL_SetRenderDrawColor(renderer, 0, 255 - r * 10, 0, 255);
//     points = generateCirclePoints(center, r, numPoints);
//     for (int x = 0; x < numPoints; x++) {
//       SDL_RenderDrawPoint(renderer, points[x].x, points[x].y);
//     }
//     free(points);
//   }
// }

// SDL_Texture *drawFilledCircle(SDL_Renderer *renderer, Vector center_vect,
//                               int radius) {
//
//   int diameter = 2 * radius;
//
//   SDL_Surface *surface =
//       SDL_CreateRGBSurface(0, diameter, diameter, 32, 0, 0, 0, 0);
//
//   SDL_LockSurface(surface);
//
//   Uint32 *pixels = (Uint32 *)surface->pixels;
//   for (int x = center_vect.x - radius; x <= center_vect.x + radius; x++) {
//     for (int y = center_vect.y - radius; y <= center_vect.y + radius; y++) {
//       int dx = x - center_vect.x;
//       int dy = y - center_vect.y;
//       if (dx * dx + dy * dy <= radius * radius) {
//         int index = (dy + radius) * diameter + (dx + radius);
//         pixels[index] = SDL_MapRGB(surface->format, 255, 0, 0);
//       }
//     }
//   }
//
//   SDL_UnlockSurface(surface);
//
//   SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
//   SDL_FreeSurface(surface);
//
//   return texture;
// }