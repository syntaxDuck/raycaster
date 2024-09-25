#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdbool.h>

#include "config.h"
#include "Structs.h"
#include "Utility.h"
#include "Actor.h"

void loadScene(Scene *Scene, Uint8 map[MAP_HEIGHT][MAP_WIDTH]);
void setSceneRenderer(SDL_Renderer *rend);
void draw2DScene(Scene Scene);
void drawMap(Scene Scene);
void drawPlayer(Player player);
void drawActor(Actor actor);
void drawActorViewDir(Actor actor);
void drawActorVelDir(Actor actor);
void drawActorViewRays(Actor actor);

void process2DScene(Scene *Scene);
void processPlayerMotion(Player *player);
void processPlayerView(Scene *Scene);

#endif
