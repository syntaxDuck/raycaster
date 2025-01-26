#ifndef ACTOR_H
#define ACTOR_H

#include <SDL.h>
#include <SDL_surface.h>
#include <stdbool.h>

#include "Vector.h"
#include "map.h"

typedef struct {
  int size;
  double field_of_view;

  double max_vel;
  double accel;

  Vector pos;
  Vector dir;
  Vector velocity;
  Vector *view_cone;
} Actor;

typedef struct Node {
  struct Node *next;
  Actor actor;
} Actor_List_Node;

void castActorRays(Actor *actor, Map map);
void processActorMotion(Actor *actor, float fps, Map map);

// DDA algo
Vector getRayRowIntersect(Vector origin, Vector ray, Map map);
Vector getRayColIntersect(Vector origin, Vector ray, Map map);
#endif
