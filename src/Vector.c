#include "Vector.h"

void printVector(Vector vector)
{
    printf("X: %f, Y: %f, Mag: %f, Angle: %f\n", vector.x, vector.y, vector.mag, vector.angle);
}

Vector normalizeVector(Vector vector)
{
    Vector norm_vect = vector;
    norm_vect.x /= vector.mag;
    norm_vect.y /= vector.mag;
    norm_vect.mag = 1;
    norm_vect.angle = vector.angle;
    return norm_vect;
}