#ifndef SCEEN_H
#define SCEEN_H

#include "Actor.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

typedef struct {
  Actor actor;

  int mag_dir;
  int plan;
} Player;

typedef struct Node {
  struct Node *next;
  Actor actor;
} Actor_List_Node;

typedef struct {
  Uint8 **map;
  Uint8 unit_size;
  Uint8 width;
  Uint8 height;
  Player player;
  Actor_List_Node actors;
} Sceen;

void loadSceen(Sceen *sceen, Uint8 **map);

void draw2DSceen(SDL_Renderer *renderer, Sceen sceen);
void drawMap(SDL_Renderer *renderer, Sceen sceen);
void drawPlayer(SDL_Renderer *renderer, Player player);
void drawActorViewDir(SDL_Renderer *renderer, Actor actor);
void drawActorVelDir(SDL_Renderer *renderer, Actor actor);
void drawActorViewRay(SDL_Renderer *renderer, Actor actor);

void process2DSceen(Sceen *sceen);
void processPlayerMotion(Player *player);
void processPlayerView(Sceen *sceen);

#endif
