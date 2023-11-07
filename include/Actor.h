#ifndef ACTOR_H
#define ACTOR_H

#include "Utility.h"

typedef struct {
  int size;
  int FOV;

  int view_distance;
  Vector ray;
  Vector vect_pos;
  Vector vect_vel;
  Vector vect_accel;
  double max_vel;
  double accel;
  double angle;
} Actor;

void createActorViewCone(Actor *actor);

void drawActor(SDL_Renderer *renderer, Actor actor);

#endif
