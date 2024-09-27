#ifndef STRUCTS_H
#define STRUCTS_H
#include <SDL2/SDL.h>

typedef struct
{
    double x;
    double y;
    double mag;
    double angle;
} Vector;

typedef struct
{
    int size;
    int number_of_rays;
    double field_of_view;

    double max_vel;
    double accel;

    Vector pos;
    Vector dir;
    Vector *view_cone;
    Vector velocity;
} Actor;

typedef struct
{
    Actor actor;
    Vector plane;
} Player;

typedef struct Node
{
    struct Node *next;
    Actor actor;
} Actor_List_Node;

typedef struct
{
    Uint8 **map;
    Uint8 unit_size;
    Uint8 width;
    Uint8 height;
    Player player;
    Actor_List_Node actors;
} Scene;
#endif