#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

#include "Actor.h"

// Env defines
#define MAP_WIDTH 8
#define MAP_HEIGHT 8
#define MAP_UNIT_SIZE 100
#define WIN_WIDTH 800
#define WIN_HEIGHT 800

// Player defines
#define PLAYER_SIZE 15
#define PLAYER_ACCEL 0.05
#define PLAYER_MAX_SPEED 1
#define PLAYER_FOV 90

typedef struct {
  const Uint8 (*map)[MAP_HEIGHT];
  Uint8 width;
  Uint8 height;
} Sceen;

// Globals
SDL_Window *win;
SDL_Renderer *rend;

Sceen *sceen;
Player *player;

// clang-format off
const Uint8 MAP[MAP_WIDTH][MAP_HEIGHT] = {
    {1, 1, 1, 1, 1, 1, 1, 1}, 
    {1, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 1}, 
    {1, 1, 1, 1, 1, 1, 1, 1}
};
// clang-format on

void drawWin(Sceen *sceen) {
  int col_offset;
  int row_offset;

  SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
  SDL_RenderClear(rend);

  for (int row = 0; row < sceen->height; row++) {
    if (row == sceen->height - 1)
      row_offset = 0;
    else
      row_offset = 1;

    for (int col = 0; col < sceen->width; col++) {

      if (col == sceen->width - 1)
        col_offset = 0;
      else
        col_offset = 1;

      if (sceen->map[row][col]) {
        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
      } else {
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
      }

      SDL_Rect rectangle = {col * (MAP_UNIT_SIZE), row * (MAP_UNIT_SIZE),
                            MAP_UNIT_SIZE - col_offset,
                            MAP_UNIT_SIZE - row_offset};
      SDL_RenderFillRect(rend, &rectangle);
    }
  }
}

void draw2D() {
  drawWin(sceen);
  drawActor(rend, &player->actor);

  SDL_RenderPresent(rend);
}

int main(int argc, char *argv[]) {
  initSDL(&win, &rend, WIN_WIDTH, WIN_HEIGHT);

  sceen = malloc(sizeof(Sceen));
  sceen->map = MAP;
  sceen->width = MAP_WIDTH;
  sceen->height = MAP_HEIGHT;

  player = malloc(sizeof(Player));
  player->actor.size = PLAYER_SIZE;
  player->actor.FOV = PLAYER_FOV;
  player->actor.max_vel = PLAYER_MAX_SPEED;
  player->actor.accel = PLAYER_ACCEL;
  player->actor.vect_pos.x = (double)WIN_WIDTH / 2 - (double)PLAYER_SIZE / 2;
  player->actor.vect_pos.y = (double)WIN_HEIGHT / 2 - (double)PLAYER_SIZE / 2;
  player->actor.vect_vel.x = 0;
  player->actor.vect_vel.y = 0;
  player->actor.vect_accel.x = 0;
  player->actor.vect_accel.y = 0;

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

    draw2D();

    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderClear(renderer);
    //
    // processSceen(renderer, sceen);
    // processActor(renderer, &player->actor);
  }

  // Cleanup and exit
  free(player);
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
