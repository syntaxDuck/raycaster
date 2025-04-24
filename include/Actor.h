#ifndef ACTOR_H
#define ACTOR_H

#include "map.h"
#include "vector.h"

typedef struct
{
  int size;
  double field_of_view;
  float turn_speed;

  double max_vel;
  double accel;

  Vector pos;
  Vector dir;
  Vector velocity;
  Vector *view_cone;
} Actor;

typedef struct Node
{
  struct Node *next;
  Actor actor;
} ActorListNode;

void cast_actor_rays(Actor *actor, Map map);
void process_actor_motion(Actor *actor, float fps, Map map);

// DDA algo
Vector get_ray_row_intersect(Vector origin, Vector ray, Map map);
Vector get_ray_col_intersect(Vector origin, Vector ray, Map map);
#endif
