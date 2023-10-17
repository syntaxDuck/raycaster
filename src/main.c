#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

#define MAP_WIDTH 8
#define MAP_HEIGHT 8
#define BLOCK_SIZE 100

// clang-format off
const bool MAP[MAP_WIDTH][MAP_HEIGHT] = {
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

int main(int argc, char *argv[]) {

  // returns zero on success else non-zero
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
  }
  SDL_Window *window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 800, 800, 0);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  bool quit = false;
  SDL_Event event;

  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int row = 0; row < sizeof(MAP) / sizeof(MAP[0][0]); row++) {
      for (int col = 0; col < sizeof(MAP[0]) / sizeof(MAP[0][0]); col++) {
        if (MAP[row][col]) {
          SDL_Rect rectangle = {col * BLOCK_SIZE, row * BLOCK_SIZE,
                                BLOCK_SIZE - 5, BLOCK_SIZE - 5};
          SDL_RenderFillRect(renderer, &rectangle);
        }
      }
    }

    SDL_RenderPresent(renderer);
  }
  // Cleanup and exit
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
