#ifndef ACTOR_H
#define ACTOR_H

#include "Utility.h"

typedef struct {
  int size;
  Vector vect_pos;
  Vector vect_vel;
  Vector vect_accel;

  int FOV;
  Vector *viewCone;

  double max_vel;
  double accel;
} Actor;

void processActorMotion(Actor *actor);

void createActorViewCone(Actor *actor);

void processActor(SDL_Renderer *renderer, Actor *actor);

#endif
