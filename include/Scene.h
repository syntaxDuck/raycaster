#ifndef SCENE_H
#define SCENE_H

#include "window.h"
#include "actor.h"
#include "map.h"
#include "player.h"
#include "sprite.h"
#include "texture.h"

#define DEFAULT_NUM_RAYS 200

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
  SceneStaticSprites static_sprites;
  SceneDynamicSprites dynamic_sprites;
  Texture *textures;
  WindowCtx *window_ctx;
} Scene;

Scene *createScene(char *map_path);

void render_scene(Scene scene, void (*render)(Scene));
void render_2d_scene(Scene scene);
void render_fp_scene(Scene scene);
void renderer_sprites(Scene scene);
void render_2d_map(Scene scene);
void render_2d_player(Scene scene, Player player);
void render_player_plane(Scene scene, Player player);
void render_player_view_rays(Scene scene, Player player);
void render_actor_body(Scene scene, Actor actor);
void render_actor_vel_dir(Scene scene, Actor actor);
void render_actor_view_dir(Scene scene, Actor actor);
void render_actor_view_rays(Scene scene, Actor actor);
void render_walls(Scene scene);
void render_floor_and_ceil(Scene scene);

void freeScene(Scene *scene);
#endif
