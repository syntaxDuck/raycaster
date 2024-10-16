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

void sortSprites(int *s_sprite_order, double *s_sprit_dist, int num_s_sprites);
#endif