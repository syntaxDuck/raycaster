#include "Defines.h"
#include "Scene.h"
#include "Texture.h"

int initSDL(WindowData *window_data, const char *title, int width, int height)
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return -1;
  }

  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
  {
    fprintf(stderr, "IMG_Init Error: %s\n", IMG_GetError());
    SDL_Quit(); // Clean up SDL
    return 1;   // or handle the error appropriately
  }

  // Create the window
  window_data->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
  if (window_data->window == NULL)
  {
    fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  // Create the renderer
  window_data->renderer = SDL_CreateRenderer(window_data->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (window_data->renderer == NULL)
  {
    fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window_data->window);
    SDL_Quit();
    return -1;
  }

  // Set the renderer color (optional)
  SDL_SetRenderDrawBlendMode(window_data->renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(window_data->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  // Initialize frame count
  window_data->title = malloc(sizeof(char) * strlen(title));
  strcpy(window_data->title, title);

  window_data->frame_count = 0;
  window_data->fps = MAX_FPS;
  window_data->last_time = SDL_GetTicks();

  return 0;
}

int main(int argc, char *argv[])
{
  WindowData window_2d, window_fp;
  // Initialize first-person rendering window and renderer
  if (REND_FP)
  {
    initSDL(&window_fp, "FP Viewport", WIN_WIDTH, WIN_HEIGHT);
  }

  // Initialize 2D rendering window and renderer
  if (REND_2D)
  {
    initSDL(&window_2d, "2D Viewport", WIN_WIDTH, WIN_HEIGHT);
  }

  // Create the scene (map and player)
  Scene *scene = createScene();
  createTextures();

  // Main game loop
  bool quit = false;
  while (!quit)
  {

    if (REND_FP)
    {
      quit = handleScene(&window_fp, *scene, renderFpScene);
    }

    if (REND_2D)
    {
      quit = handleScene(&window_2d, *scene, render2dScene);
    }

    processPlayerMotion(&scene->player, 1 / window_fp.fps, scene->map);
  }

  // Cleanup and exit
  freeScene(scene);
  freeWindowData(&window_2d);
  freeWindowData(&window_fp);
  SDL_Quit();
  return 0;
}
