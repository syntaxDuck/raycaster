#ifndef ACTOR_H
#define ACTOR_H

#include "Utility.h"

typedef struct {
  int size;
  int FOV;

  double max_vel;
  double accel;

  Point pos;

  Vector vect_view;
  Vector view_ray;
  Vector vect_vel;
  Vector vect_accel;
} Actor;

void createActorViewCone(Actor *actor);

void drawActor(SDL_Renderer *renderer, Actor actor);

#endif
