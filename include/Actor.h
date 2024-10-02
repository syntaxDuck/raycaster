#ifndef ACTOR_H
#define ACTOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <stdbool.h>

#include "Defines.h"
#include "Structs.h"
#include "Vector.h"

void castPlayerRays(Player *player, Scene scene);
void freePlayer(Player *player);

void castActorRays(Actor *actor, Scene scene);
void processActorMotion(Actor *actor, float fps, Uint8 **walls);

// DDA algo
WallIntersect getIntersect(Vector origin, Vector ray, Scene scene);
Vector getRayRowIntersect(Vector origin, Vector ray, Scene scene);
Vector getRayColIntersect(Vector origin, Vector ray, Scene scene);
#endif
