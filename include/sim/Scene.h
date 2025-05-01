#ifndef SCENE_H
#define SCENE_H

#include "window.h"
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
  SceneStaticSprites static_sprites;
  SceneDynamicSprites dynamic_sprites;
  Texture *textures;
  WindowCtx *window_ctx;
} Scene;

Scene *create_scene(WindowCtx *window_ctx, char *map_path);
void set_current_scene(Scene *scene);
Scene *get_current_scene();
void free_scene(Scene *scene);

void render_scene(void (*render)());
void render_2d_scene();
void render_fp_scene();
void renderer_sprites();
void render_2d_map();
void render_2d_player(Player player);
void render_player_plane(Player player);
void render_player_view_rays(Player player);
void render_actor_body(Actor actor);
void render_actor_vel_dir(Actor actor);
void render_actor_view_dir(Actor actor);
void render_actor_view_rays(Actor actor);
void render_walls();
void render_floor_and_ceil();

#endif
