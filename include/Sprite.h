#ifndef SPRITE_H
#define SPRITE_H

#include "Actor.h"
#include "Vector.h"

typedef struct
{
    Vector pos;
    int texture;
} StaticSprit;

typedef struct
{
    Actor actor;
    int texture;
} DynamicSprit;
#endif