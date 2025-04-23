#ifndef SCENE_H
#define SCENE_H

#include <SDL.h>
#include <SDL_render.h>
#include <SDL_image.h>
#include <stdbool.h>

#include "actor.h"
#include "map.h"
#include "player.h"
#include "sprite.h"
#include "texture.h"

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

void render_scene(Scene scene, void (*render)(Scene));
void render_2d_scene(Scene scene);
void render_fp_scene(Scene scene);
void renderer_sprites(Scene scene);
void render_2d_map(Scene Scene);
void render_2d_player(Player player);
void render_player_plane(Player player);
void render_player_view_rays(Player player);
void render_actor_body(Actor actor);
void render_actor_vel_dir(Actor actor);
void render_actor_view_dir(Actor actor);
void render_actor_view_rays(Actor actor);
void render_walls(Scene scene);
void render_floor_and_ceil(Scene scene);

void freeScene(Scene *scene);
#endif
