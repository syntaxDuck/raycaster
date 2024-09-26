#ifndef STRUCTS_H
#define STRUCTS_H
#include <SDL2/SDL.h>

typedef struct
{
    double x;
    double y;
    double z;
} Point;

typedef struct
{
    Point point;
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

    Point pos;

    Vector vect_view;
    Vector *view_cone;
    Vector view_ray;
    Vector vect_vel;
    Vector vect_accel;
} Actor;

typedef struct
{
    Actor actor;

    int mag_dir;
    int plan;
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