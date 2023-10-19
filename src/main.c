#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>

#define MAP_WIDTH 8
#define MAP_HEIGHT 8
#define SCREEN_WIDTH 800
#define SCREEN_HIGHT 800
#define PLAYER_ACCEL 0.05
#define PLAYER_MAX_SPEED 1

// clang-format off
const u_int8_t MAP[MAP_WIDTH][MAP_HEIGHT] = {
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

const int BLOCK_PAD = 5;
const int BLOCK_SIZE = SCREEN_WIDTH / MAP_WIDTH - BLOCK_PAD;

int main(int argc, char *argv[]) {

  int windowWidth = SCREEN_WIDTH, windowHeight = SCREEN_HIGHT;

  // returns zero on success else non-zero
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("error initializing SDL: %s\n", SDL_GetError());
  }
  SDL_Window *window =
      SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       windowWidth, windowHeight, 0);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  int playerX = SCREEN_WIDTH / 2, playerY = SCREEN_HIGHT / 2;
  float speedX = 0, speedY = 0;

  bool quit = false;
  SDL_Event event;

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

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int row = 0; row < sizeof(MAP) / sizeof(MAP[0]); row++) {
      for (int col = 0; col < sizeof(MAP[0]) / sizeof(MAP[0][0]); col++) {
        if (MAP[row][col]) {
          SDL_Rect rectangle = {col * (BLOCK_SIZE + BLOCK_PAD),
                                row * (BLOCK_SIZE + BLOCK_PAD), BLOCK_SIZE,
                                BLOCK_SIZE};
          SDL_RenderFillRect(renderer, &rectangle);
        }
      }
    }

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    // Horizontal Velocity Function
    if (state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]) {
      if (speedX > -PLAYER_MAX_SPEED)
        speedX -= PLAYER_ACCEL;
    } else if (state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT]) {
      if (speedX < PLAYER_MAX_SPEED)
        speedX += PLAYER_ACCEL;
    } else {
      // Gradual deceleration to stop
      if (speedX > 0) {
        speedX -= PLAYER_ACCEL;
        if (speedX < 0)
          speedX = 0;
      } else if (speedX < 0) {
        speedX += PLAYER_ACCEL;
        if (speedX > 0)
          speedX = 0;
      }
    }
    // Verical Velocity Function
    if (state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN]) {
      if (speedY > -PLAYER_MAX_SPEED)
        speedY -= PLAYER_ACCEL;
    } else if (state[SDL_SCANCODE_DOWN] && !state[SDL_SCANCODE_UP]) {
      if (speedY < PLAYER_MAX_SPEED)
        speedY += PLAYER_ACCEL;
    } else {
      // Gradual deceleration to stop
      if (speedY > 0) {
        speedY -= PLAYER_ACCEL;
        if (speedY < 0)
          speedY = 0;
      } else if (speedY < 0) {
        speedY += PLAYER_ACCEL;
        if (speedY > 0)
          speedY = 0;
      }
    }

    playerX += speedX;
    playerY += speedY;

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect player = {playerX, playerY, 5, 5};
    SDL_RenderFillRect(renderer, &player);

    SDL_RenderPresent(renderer);
  }
  // Cleanup and exit
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
