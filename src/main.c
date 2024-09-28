#include <SDL2/SDL.h>

#include "Defines.h"
#include "Structs.h"
#include "Scene.h"

void setupPlayer(Player *player)
{
  player->actor.size = PLAYER_SIZE;
  player->actor.field_of_view = PLAYER_FOV * DEG_TO_RAD;

  player->actor.view_cone = malloc(sizeof(Vector) * WIN_WIDTH);
  player->actor.max_vel = PLAYER_MAX_SPEED;
  player->actor.accel = PLAYER_ACCEL;

  player->actor.pos = setVector(2 * (double)WIN_WIDTH / 3 - 1, 2 * (double)WIN_HEIGHT / 3 - 1, 0, 0);
  player->actor.velocity = setVector(0, 0, 0, 0);
  player->actor.velocity = setVector(-1, 0, 1, 0);

  double y = player->actor.dir.x * tan(player->actor.field_of_view / 2);
  player->plane = setVector(0, y, y, M_PI_2);
}

bool handle2dScene(Scene *scene_2d, SDL_Window *win_2d)
{
  SDL_Event event;
  bool quit = false;
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
        SDL_SetWindowSize(win_2d, newWidth, newHeight);
      }
    }
  }

  draw2DScene(*scene_2d);
  return quit;
}

bool handleFpScene(Scene *scene_fp, SDL_Window *win_fp)
{
  SDL_Event event;
  bool quit = false;
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
        SDL_SetWindowSize(win_fp, newWidth, newHeight);
      }
    }
  }

  drawFpScene(scene_fp);
  return quit;
}

int main(int argc, char *argv[])
{
  // Init window and renderer
  SDL_Window *win_2d = NULL, *win_fp = NULL;
  SDL_Renderer *rend_2d = NULL, *rend_fp = NULL;

  // Init Scene Object
  Scene *scene_2d = malloc(sizeof(Scene));
  Scene *scene_fp = malloc(sizeof(Scene));

  Map map;
  map.unit_size = MAP_UNIT_SIZE;
  map.grid = loadMapFromFile("../assets/maps/map.txt", &map.width, &map.height);

  if (map.grid == NULL)
  {
    fprintf(stderr, "Failed to load map from file\n");
    exit(EXIT_FAILURE);
  }

  // Create Player
  Player *player = malloc(sizeof(Player)); // Allocate memory for the player
  if (!player)
  {
    fprintf(stderr, "Failed to allocate memory for player\n");
    exit(EXIT_FAILURE);
  }

  setupPlayer(player);

  if (REND_FP)
  {
    scene_fp->map = map;
    scene_fp->player = *player;
    initSDL(&win_fp, &rend_fp, WIN_WIDTH, WIN_HEIGHT);
    SDL_SetRenderDrawBlendMode(rend_fp, SDL_BLENDMODE_BLEND);
    setFpSceneRenderer(rend_fp);
  }

  if (REND_2D)
  {
    scene_2d->map = map;
    scene_2d->player = *player;
    initSDL(&win_2d, &rend_2d, WIN_WIDTH, WIN_HEIGHT);
    SDL_SetRenderDrawBlendMode(rend_2d, SDL_BLENDMODE_BLEND);
    set2dSceneRenderer(rend_2d);
  }

  bool quit = false;
  while (!quit)
  {
    if (REND_2D)
    {
      quit = handle2dScene(scene_2d, win_2d);
    }

    if (REND_FP)
    {
      quit = handleFpScene(scene_fp, win_fp);
    }

    processPlayerMotion(scene_2d);
  }

  // Cleanup and exit
  free(player->actor.view_cone);
  free(player);
  freeMap(map.grid, map.height);
  free(scene_2d);
  free(scene_fp);

  if (rend_2d)
    SDL_DestroyRenderer(rend_2d);
  if (win_2d)
    SDL_DestroyWindow(win_2d);
  if (rend_fp)
    SDL_DestroyRenderer(rend_fp);
  if (win_fp)
    SDL_DestroyWindow(win_fp);

  SDL_Quit();
  return 0;
}
