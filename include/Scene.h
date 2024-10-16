#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL_image.h>
#include <math.h>
#include <stdbool.h>

#include "Config.h"
#include "Global.h"
#include "Window.h"
#include "Texture.h"
#include "Vector.h"
#include "Player.h"
#include "Actor.h"
#include "Sprite.h"
#include "Map.h"

typedef struct
{
    int num_sprites;
    int *sprite_order;
    double *sprite_dist;
    StaticSprite *sprites;
} SceneStaticSprites;

typedef struct
{
    int num_sprites;
    int *sprite_order;
    double *sprite_dist;
    DynamicSprite *sprites;
} SceneDynamicSprites;

typedef struct
{
    Map map;
    Player player;
    SceneStaticSprites s_sprites;
    SceneDynamicSprites d_sprites;

    Texture *textures;
} Scene;

Scene *createScene(char *map_path);

void renderScene(Scene scene, void (*render)(Scene));
void render2dScene(Scene scene);
void renderFpScene(Scene scene);
void rendererSprites(Scene scene);
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
