#ifndef PLAYER_H
#define PLAYER_H
#include "Actor.h"
#include "Vector.h"

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

Player createPlayer();
void freePlayer(Player *player);
void castPlayerRays(Player *player, Map map);
void processPlayerMotion(Player *player, float fps, Map map);
WallIntersect getWallIntersect(Vector origin, Vector ray, Map map);
#endif