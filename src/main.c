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
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

// Player defines
#define PLAYER_SIZE 15
#define PLAYER_ACCEL 0.05
#define PLAYER_MAX_SPEED 1

// clang-format off
const Uint8 MAP[MAP_WIDTH][MAP_HEIGHT] = {
    {1, 1, 1, 1, 1, 1, 1, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 1, 1, 1, 1, 1, 1, 1}
};
// clang-format on

typedef struct {
  const Uint8 (*map)[MAP_HEIGHT];
  Uint8 width;
  Uint8 height;
} Sceen;

void processSceen(SDL_Renderer *renderer, Sceen sceen) {
  const int BLOCK_PAD = 0;
  const int BLOCK_SIZE = SCREEN_WIDTH / MAP_WIDTH - BLOCK_PAD;

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  for (int row = 0; row < sceen.height; row++) {
    for (int col = 0; col < sceen.width; col++) {
      if (sceen.map[row][col]) {
        SDL_Rect rectangle = {col * (BLOCK_SIZE + BLOCK_PAD),
                              row * (BLOCK_SIZE + BLOCK_PAD), BLOCK_SIZE,
                              BLOCK_SIZE};
        SDL_RenderFillRect(renderer, &rectangle);
      }
    }
  }
}

int main(int argc, char *argv[]) {

  initSDL();
  SDL_Window *window = initSDLWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_Renderer *renderer = initSDLRenderer(window);

  Sceen sceen;
  sceen.map = MAP;
  sceen.width = MAP_WIDTH;
  sceen.height = MAP_HEIGHT;

  Actor *player = malloc(sizeof(Actor));
  player->size = PLAYER_SIZE;
  player->max_vel = PLAYER_MAX_SPEED;
  player->accel = PLAYER_ACCEL;
  player->vect_pos.x = (double)SCREEN_WIDTH / 2;
  player->vect_pos.y = (double)SCREEN_HEIGHT / 2;
  player->vect_vel.x = 0;
  player->vect_vel.y = 0;
  player->vect_accel.x = 0;
  player->vect_accel.y = 0;
  player->FOV = 90;

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
          SDL_SetWindowSize(window, newWidth, newHeight);
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    processSceen(renderer, sceen);
    processActor(renderer, player);

    SDL_RenderPresent(renderer);
  }

  // Cleanup and exit
  free(player);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
