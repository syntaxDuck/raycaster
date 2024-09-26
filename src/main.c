#include <SDL2/SDL.h>

#include "Defines.h"
#include "Structs.h"
#include "Scene.h"

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

void setupPlayer(Player *player)
{
  player->actor.size = PLAYER_SIZE;
  player->actor.field_of_view = PLAYER_FOV * DEG_TO_RAD;
  player->actor.number_of_rays = PLAYER_RAYS;

  player->actor.view_cone = malloc(sizeof(Vector) * PLAYER_RAYS);
  player->actor.max_vel = PLAYER_MAX_SPEED;
  player->actor.accel = PLAYER_ACCEL;

  setPoint(&player->actor.pos, (double)WIN_WIDTH / 2 - 1, (double)WIN_HEIGHT / 2 - 1);
  setVector(&player->actor.vect_vel, 0, 0, 0, 0);
  setVector(&player->actor.vect_accel, 0, 0, 0, 0);
  setVector(&player->actor.vect_view, -10, 0, 10, 0);
}

int main(int argc, char *argv[])
{
  // Init window and renderer
  SDL_Window *win;
  SDL_Renderer *rend;
  initSDL(&win, &rend, WIN_WIDTH, WIN_HEIGHT);
  setSceneRenderer(rend);

  // Init Scene Object
  Scene *Scene = malloc(sizeof(Scene));
  Scene->unit_size = MAP_UNIT_SIZE;
  Scene->width = MAP_WIDTH;
  Scene->height = MAP_HEIGHT;
  loadScene(Scene, MAP);

  // Create Player
  Player *player;
  setupPlayer(player);
  Scene->player = *player;

  bool quit = false;
  SDL_Event event;
  while (!quit)
  {
    while (SDL_PollEvent(&event) != 0)
    {
      if (event.type == SDL_QUIT)
      {
        quit = true;
      }
      else if (event.type == SDL_WINDOWEVENT)
      {
        if (event.window.type == SDL_WINDOWEVENT_RESIZED)
        {
          int newWidth = event.window.data1;
          int newHeight = event.window.data2;
          SDL_SetWindowSize(win, newWidth, newHeight);
        }
      }
    }

    process2DScene(Scene);
    draw2DScene(*Scene);
  }

  // Cleanup and exit
  free(player->actor.view_cone);
  free(Scene);
  SDL_DestroyRenderer(rend);
  SDL_DestroyWindow(win);
  SDL_Quit();
  return 0;
}
