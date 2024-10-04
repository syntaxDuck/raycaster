#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL_image.h>
#include <math.h>
#include <stdbool.h>

#include "Defines.h"
#include "Vector.h"
#include "Player.h"
#include "Actor.h"
#include "Map.h"

typedef struct
{
    Map map;
    Player player;
    Actor_List_Node actors;
} Scene;

typedef struct
{
    char *title;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int frame_count;
    float fps;
    Uint32 last_time;
} WindowData;

Scene *createScene();
bool handleScene(WindowData *window_data, Scene scene, void (*draw)(Scene, SDL_Renderer *));
void updateFrameCounter(WindowData *window_data);
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
void renderWalls(Player player, Map map);
void renderFloorAndCeil(Player player, Map map);

void freeScene(Scene *scene);
void freeWindowData(WindowData *window_data);
#endif
