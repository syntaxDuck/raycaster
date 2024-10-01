#include "Vector.h"

// Creates and returns a vector with the given x and y coordinates.
// It calculates the vector's angle and magnitude.
Vector setVector(double x, double y)
{
    Vector vect;
    vect.x = x;
    vect.y = y;
    calculateVectorAngle(&vect); // Calculate angle based on x and y
    calculateVectorMag(&vect);   // Calculate magnitude based on x and y
    return vect;
}

// Creates and returns a point with the given x and y coordinates.
// Magnitude and angle are set to NAN since this is just a point, not a full vector.
Vector setPoint(double x, double y)
{
    Vector vect;
    vect.x = x;
    vect.y = y;
    vect.mag = NAN;   // Points do not have a defined magnitude
    vect.angle = NAN; // Points do not have a defined angle
    return vect;
}

// Sets the magnitude of the given vector and updates its x and y components accordingly.
// If magnitude is zero, x and y are set to zero, and the angle is set to NAN.
void setVectorMagnitude(Vector *vector, double mag)
{
    vector->mag = mag;
    if (mag != 0)
    {
        vector->x = mag * cos(vector->angle); // Update x based on the new magnitude and angle
        vector->y = mag * sin(vector->angle); // Update y based on the new magnitude and angle
    }
    else
    {
        vector->x = 0; // If magnitude is zero, x and y must be zero
        vector->y = 0;
        vector->angle = NAN; // Angle is undefined for a zero-magnitude vector
    }
}

// Calculates and sets the magnitude of the given vector based on its x and y components.
void calculateVectorMag(Vector *vector)
{
    vector->mag = sqrt(vector->x * vector->x + vector->y * vector->y); // Pythagorean theorem
}

// Calculates and sets the angle of the given vector based on its x and y components.
// Ensures the angle is normalized to the range [0, 2π).
void calculateVectorAngle(Vector *vector)
{
    vector->angle = atan2(vector->y, vector->x);        // Calculate angle using atan2 for full 360° range
    vector->angle = fmod(vector->angle + M_2PI, M_2PI); // Normalize to [0, 2π)
}

// Prints the vector's components, magnitude, and angle to the console.
void printVector(Vector vector)
{
    printf("X: %f, Y: %f, Mag: %f, Angle: %f\n", vector.x, vector.y, vector.mag, vector.angle);
}

// Normalizes the vector to a unit vector (magnitude = 1) if its magnitude is not zero.
// If the vector has zero magnitude, its components are set to zero and the angle to NAN.
Vector normalizeVector(Vector vector)
{
    Vector norm_vect = vector;
    if (vector.mag == 0)
    {
        norm_vect.x = 0;
        norm_vect.y = 0;
        norm_vect.mag = 0;
        norm_vect.angle = NAN; // Undefined angle for zero-magnitude vector
    }
    else
    {
        norm_vect.x /= vector.mag;        // Normalize x component
        norm_vect.y /= vector.mag;        // Normalize y component
        norm_vect.mag = 1;                // Set magnitude to 1 for a unit vector
        calculateVectorAngle(&norm_vect); // Recalculate the angle
    }

    return norm_vect;
}

// Rotates the vector by the given angle (in radians).
// Updates the vector's x and y components and ensures the angle is normalized to [0, 2π).
void rotateVector(Vector *vector, double angle)
{
    double newX = vector->x * cos(angle) - vector->y * sin(angle); // Rotation matrix for x
    double newY = vector->x * sin(angle) + vector->y * cos(angle); // Rotation matrix for y
    vector->x = newX;
    vector->y = newY;

    vector->angle += angle;                             // Update the vector's angle by adding the rotation
    vector->angle = fmod(vector->angle + M_2PI, M_2PI); // Normalize the angle to [0, 2π)
}
