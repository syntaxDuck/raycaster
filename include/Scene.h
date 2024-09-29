#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdbool.h>

#include "Defines.h"
#include "Structs.h"
#include "Utility.h"
#include "Actor.h"

Uint8 **loadMapFromFile(const char *filename, int *map_width, int *map_height);
void printMap(Uint8 **map, int map_width, int map_height);
void freeMap(Uint8 **map, int map_height);
void freeScene(Scene *scene);
void set2dSceneRenderer(SDL_Renderer *rend);
void setFpSceneRenderer(SDL_Renderer *rend);

void draw2dScene(Scene scene, SDL_Renderer *rend);
void draw2dMap(Scene Scene);
void draw2dPlayer(Player player);
void drawPlayerPlane(Player player);
void drawPlayerViewRays(Player player);
void drawActor(Actor actor);
void drawActorVelDir(Actor actor);
void drawActorViewDir(Actor actor);
void drawActorViewRays(Actor actor);

void drawFpScene(Scene scene, SDL_Renderer *rend);
void drawWalls(Player player, Scene scene);
// void drawVectorFromActor(Actor actor, Vector vect, int scale, long color);

void process2DScene(Scene *scene);
void processPlayerView(Scene *scene);
void processPlayerMotion(Scene *scene);

#endif
