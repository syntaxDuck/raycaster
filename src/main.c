#include <SDL2/SDL.h>

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

#include "Actor.h"
#include "Sceen.h"
#include "Utility.h"

// Env defines
#define MAP_WIDTH 8
#define MAP_HEIGHT 8
#define MAP_UNIT_SIZE 64
#define WIN_WIDTH MAP_UNIT_SIZE *MAP_WIDTH
#define WIN_HEIGHT MAP_UNIT_SIZE *MAP_HEIGHT

// Player defines
#define PLAYER_SIZE 5
#define PLAYER_ACCEL 0.05
#define PLAYER_MAX_SPEED 1
#define PLAYER_FOV 90

// Globals

// clang-format off
Uint8 MAP[MAP_WIDTH][MAP_HEIGHT] = {
    {1, 0, 1, 0, 1, 0, 1, 0}, 
    {0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 1, 0, 1, 0}, 
    {0, 1, 0, 1, 0, 1, 0, 1}
};
// clang-format on

void setupPlayer(Player *player) {
  player->actor.size = PLAYER_SIZE;
  player->actor.FOV = PLAYER_FOV;

  player->actor.max_vel = PLAYER_MAX_SPEED;
  player->actor.accel = PLAYER_ACCEL;

  player->actor.pos.x = (double)WIN_WIDTH / 2 - 1;
  player->actor.pos.y = (double)WIN_HEIGHT / 2 - 1;

  player->actor.vect_vel.point.x = 0;
  player->actor.vect_vel.point.y = 0;
  player->actor.vect_vel.mag = 0;
  player->actor.vect_vel.angle = 0;

  player->actor.vect_accel.point.x = 0;
  player->actor.vect_accel.point.y = 0;
  player->actor.vect_accel.mag = 0;
  player->actor.vect_accel.angle = 0;

  player->actor.vect_view.point.x = -10;
  player->actor.vect_view.point.y = 0;
  player->actor.vect_view.mag = 10;
  player->actor.vect_view.angle = 0;

  player->actor.view_ray = player->actor.vect_view;
  player->actor.view_ray.point.x = -1000;
  player->actor.view_ray.mag = 1000;
}

int main(int argc, char *argv[]) {

  // Init window and renderer
  SDL_Window *win;
  SDL_Renderer *rend;
  initSDL(&win, &rend, WIN_WIDTH, WIN_HEIGHT);

  // Init Sceen Object
  Sceen *sceen = malloc(sizeof(Sceen));
  sceen->unit_size = MAP_UNIT_SIZE;
  sceen->width = MAP_WIDTH;
  sceen->height = MAP_HEIGHT;

  sceen->map = malloc(sceen->height * sizeof(Uint8 *));
  for (int i = 0; i < sceen->height; i++) {
    sceen->map[i] = malloc(sceen->width * sizeof(Uint8));
  }

  for (int row = 0; row < sceen->width; row++) {
    for (int col = 0; col < sceen->width; col++) {
      sceen->map[row][col] = MAP[row][col];
    }
  }

  // Create Player
  Player player;
  setupPlayer(&player);

  // Assign player to sceen
  sceen->player = player;

  bool quit = false;
  SDL_Event event;

  // Main Loop //
  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        quit = true;
      } else if (event.type == SDL_WINDOWEVENT) {
        if (event.window.type == SDL_WINDOWEVENT_RESIZED) {
          int newWidth = event.window.data1;
          int newHeight = event.window.data2;
          SDL_SetWindowSize(win, newWidth, newHeight);
        }
      }
    }

    process2DSceen(sceen);
    draw2DSceen(rend, *sceen);
  }

  // Cleanup and exit
  free(sceen);
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
