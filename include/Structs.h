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
    double field_of_view;

    double max_vel;
    double accel;

    Vector pos;
    Vector dir;
    Vector velocity;
    Vector *view_cone;
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
    Uint8 **grid;
    int unit_size;
    int width;
    int height;
} Map;

typedef struct
{
    Map map;
    Player player;
    Actor_List_Node actors;
} Scene;

#endif