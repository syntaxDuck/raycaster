#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL_image.h>
#include <math.h>
#include <stdbool.h>

#include "Defines.h"
#include "Texture.h"
#include "Vector.h"
#include "Player.h"
#include "Actor.h"
#include "Map.h"

typedef struct
{
    Map map;
    Player player;
    Texture *textures;
} Scene;

Scene *createScene();
void set2dSceneRenderer(SDL_Renderer *rend);
void setFpSceneRenderer(SDL_Renderer *rend);

void render2dScene(Scene scene, SDL_Renderer *rend);
void renderFpScene(Scene scene, SDL_Renderer *rend);
void render2dMap(Scene Scene);
void render2dPlayer(Player player);
void renderPlayerPlane(Player player);
void renderPlayerViewRays(Player player);
void renderActorBody(Actor actor);
void renderActorVelDir(Actor actor);
void renderActorViewDir(Actor actor);
void renderActorViewRays(Actor actor);
void renderWalls(Scene scene);
void renderFloorAndCeil(Scene scene);

void freeScene(Scene *scene);
#endif
