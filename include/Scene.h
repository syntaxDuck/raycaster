#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdbool.h>

#include "Defines.h"
#include "Vector.h"
#include "Map.h"
#include "Structs.h"
#include "Actor.h"

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
void renderWalls(Player player, Map map);
void renderFloorAndCeil(Player player, Map map);

void process2DScene(Scene *scene);
void processPlayerView(Scene *scene);
void processPlayerMotion(Scene *scene, float fps, Uint8 **grid);

#endif
