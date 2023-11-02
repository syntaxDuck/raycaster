#ifndef ACTOR_H
#define ACTOR_H

#include "Utility.h"

typedef struct {
  int size;
  int FOV;
  Vector *view_cone;
  Vector vect_pos;
  Vector vect_vel;
  Vector vect_accel;
  double max_vel;
  double accel;
} Actor;

typedef struct {
  Actor actor;
  int mag_dir;
  int plan;
} Player;

void processActorMotion(Actor *actor);

void createActorViewCone(Actor *actor);

void drawActor(SDL_Renderer *renderer, Actor *actor);

#endif
