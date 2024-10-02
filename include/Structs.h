#ifndef STRUCTS_H
#define STRUCTS_H
#include "Vector.h"
#include "Map.h"

typedef struct
{
    char *title;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int frame_count;
    float fps;
    Uint32 last_time;
} WindowData;

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
    double perp_wall_distance;
    int side;
    int map_x;
    int map_y;
    Vector vect;
    Vector ray_dir;
} WallIntersect;

typedef struct
{
    Actor actor;
    Vector plane;
    WallIntersect *intersects;
} Player;

typedef struct Node
{
    struct Node *next;
    Actor actor;
} Actor_List_Node;

typedef struct
{
    Map map;
    Player player;
    Actor_List_Node actors;
} Scene;

#endif