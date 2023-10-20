#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
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
#define PLAYER_SIZE 5
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
  double x;
  double y;
  double z;
} Vector;

typedef struct {
  Vector pos;
  Vector velocity;
  Vector accel;
} Actor;

typedef struct {
  const Uint8 (*map)[MAP_HEIGHT];
  Uint8 width;
  Uint8 height;
} Sceen;

void processActorMotion(Actor *actor) {
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  // Horizontal Velocity Function
  if (state[SDL_SCANCODE_LEFT] && !state[SDL_SCANCODE_RIGHT]) {
    if (actor->velocity.x > -PLAYER_MAX_SPEED)
      actor->velocity.x -= PLAYER_ACCEL;
  } else if (state[SDL_SCANCODE_RIGHT] && !state[SDL_SCANCODE_LEFT]) {
    if (actor->velocity.x < PLAYER_MAX_SPEED)
      actor->velocity.x += PLAYER_ACCEL;
  } else {
    // Gradual deceleration to stop
    if (actor->velocity.x > 0) {
      actor->velocity.x -= PLAYER_ACCEL;
      if (actor->velocity.x < 0)
        actor->velocity.x = 0;
    } else if (actor->velocity.x < 0) {
      actor->velocity.x += PLAYER_ACCEL;
      if (actor->velocity.x > 0)
        actor->velocity.x = 0;
    }
  }
  // Verical Velocity Function
  if (state[SDL_SCANCODE_UP] && !state[SDL_SCANCODE_DOWN]) {
    if (actor->velocity.y > -PLAYER_MAX_SPEED)
      actor->velocity.y -= PLAYER_ACCEL;
  } else if (state[SDL_SCANCODE_DOWN] && !state[SDL_SCANCODE_UP]) {
    if (actor->velocity.y < PLAYER_MAX_SPEED)
      actor->velocity.y += PLAYER_ACCEL;
  } else {
    // Gradual deceleration to stop
    if (actor->velocity.y > 0) {
      actor->velocity.y -= PLAYER_ACCEL;
      if (actor->velocity.y < 0)
        actor->velocity.y = 0;
    } else if (actor->velocity.y < 0) {
      actor->velocity.y += PLAYER_ACCEL;
      if (actor->velocity.y > 0)
        actor->velocity.y = 0;
    }
  }

  actor->pos.x += actor->velocity.x;
  actor->pos.y += actor->velocity.y;
}

void processSceen(Sceen sceen, SDL_Renderer *renderer) {
  const int BLOCK_PAD = 0;
  const int BLOCK_SIZE = SCREEN_WIDTH / MAP_WIDTH - BLOCK_PAD;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

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

void initSDL() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    printf("Error initializing SDL: %s\n", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
}

SDL_Window *initSDLWindow() {
  SDL_Window *window =
      SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       SCREEN_WIDTH, SCREEN_HIGHT, 0);
  if (window == NULL) {
    printf("Error creating SDL window: %s\n", SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  return window;
}

SDL_Renderer *initSDLRenderer(SDL_Window *window) {
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    printf("Error creating SLD renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  return renderer;
}

int main(int argc, char *argv[]) {

  initSDL();
  SDL_Window *window = initSDLWindow();
  SDL_Renderer *renderer = initSDLRenderer(window);

  int windowWidth = SCREEN_WIDTH, windowHeight = SCREEN_HIGHT;

  Sceen sceen;
  sceen.map = MAP;
  sceen.width = MAP_WIDTH;
  sceen.height = MAP_HEIGHT;

  Actor player;
  player.pos.x = (double)SCREEN_WIDTH / 2;
  player.pos.y = (double)SCREEN_HIGHT / 2;
  player.velocity.x = 0;
  player.velocity.y = 0;
  player.accel.x = 0;
  player.accel.y = 0;

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

    processSceen(sceen, renderer);
    processActorMotion(&player);

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect player_rend = {player.pos.x, player.pos.y, PLAYER_SIZE,
                            PLAYER_SIZE};
    SDL_RenderFillRect(renderer, &player_rend);

    SDL_RenderPresent(renderer);
  }

  // Cleanup and exit
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
