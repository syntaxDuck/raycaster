#ifndef ACTOR_H
#define ACTOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <stdbool.h>

#include "Defines.h"
#include "Structs.h"
#include "Utility.h"

void castActorRays(Actor *actor, Scene Scene);
void processActorMotion(Actor *actor);
Vector getRayRowIntersect(Vector origin, Vector ray, Scene scene);
Vector getRayColIntersect(Vector origin, Vector ray, Scene scene);
#endif
