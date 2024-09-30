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

void render2dScene(Scene scene, SDL_Renderer *rend);
void render2dMap(Scene Scene);
void render2dPlayer(Player player);
void renderPlayerPlane(Player player);
void renderPlayerViewRays(Player player);
void renderActorBody(Actor actor);
void renderActorVelDir(Actor actor);
void renderActorViewDir(Actor actor);
void renderActorViewRays(Actor actor);

void drawFpScene(Scene scene, SDL_Renderer *rend);
void drawWalls(Player player, Scene scene);
void renderWalls(Player player, Scene scene);
// void drawVectorFromActor(Actor actor, Vector vect, int scale, long color);

void process2DScene(Scene *scene);
void processPlayerView(Scene *scene);
void processPlayerMotion(Scene *scene, float fps);

#endif
