#ifndef SCEEN_H
#define SCEEN_H

#include "Actor.h"
#include <SDL2/SDL.h>

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

void draw2DSceen(SDL_Renderer *renderer, Sceen *sceen);

void drawMap(SDL_Renderer *renderer, Sceen *sceen);

void processPlayerMotion(Player *player);

#endif
