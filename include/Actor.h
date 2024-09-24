#ifndef ACTOR_H
#define ACTOR_H

#include "Structs.h"
#include "Utility.h"

void createActorViewCone(Actor *actor);
void processActorView(Actor *actor, Scene Scene);
void processActorMotion(Actor *actor);
Vector getRayRowIntersect(Point origin, Vector ray, Scene scene);
Vector getRayColIntersect(Point origin, Vector ray, Scene scene);
#endif
