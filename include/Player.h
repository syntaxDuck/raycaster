#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"
#include "vector.h"

typedef struct {
  double perp_wall_distance;
  int side;
  int map_x;
  int map_y;
  Vector vect;
  Vector ray_dir;
} WallIntersect;

typedef struct {
  Actor actor;
  Vector plane;
  WallIntersect *intersects;
} Player;

Player create_player();
void free_player(Player *player);
void cast_player_rays(Player *player, Map map);
void process_player_motion(Player *player, float fps, Map map);
WallIntersect get_wall_intersect(Vector origin, Vector ray, Map map);
#endif
