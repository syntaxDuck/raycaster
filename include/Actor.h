#ifndef ACTOR_H
#define ACTOR_H

#include "Utility.h"

typedef struct {
  int size;
  int FOV;

  Pos_Vector ray;
  Pos_Vector vect_pos;

  Vector vect_vel;
  Vector vect_accel;
  double max_vel;
  double accel;
} Actor;

void createActorViewCone(Actor *actor);

void drawActor(SDL_Renderer *renderer, Actor actor);

Pos_Vector getActorCenter(Actor actor);

#endif
