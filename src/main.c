#include "Defines.h"
#include "Scene.h"
#include "Texture.h"
#include "Window.h"

int main(int argc, char *argv[])
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr,
            "SDL could not initialize! SDL_Error: %s\n",
            SDL_GetError());
    return -1;
  }

  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
  {
    fprintf(stderr, "IMG_Init Error: %s\n", IMG_GetError());
    SDL_Quit(); // Clean up SDL
    return 1;   // or handle the error appropriately
  }

  WindowData *window_2d;
  WindowData *window_main = createWindow("Main Viewport",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         WIN_WIDTH, WIN_HEIGHT);

  // Create the scene (map and player)
  Scene *scene = createScene();
  createTextures();

  // Main game loop
  SDL_Event event;
  bool quit = false;
  bool show_2d = false;

  const Uint8 *state;
  bool key_pressed = false;
  while (!quit)
  {
    // Handle events
    while (SDL_PollEvent(&event) != 0)
    {
      if (event.type == SDL_QUIT)
      {
        quit = true;
      }
      if (event.type == SDL_KEYDOWN && !key_pressed)
      {
        key_pressed = true;
        state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_2])
        {
          if (!show_2d)
          {
            int x, y;
            SDL_GetWindowPosition(window_main->window, &x, &y);
            window_2d = createWindow("2D Viewport",
                                     x - WIN_WIDTH,
                                     y, WIN_WIDTH,
                                     WIN_HEIGHT);
            show_2d = true;
          }
          else
          {
            freeWindowData(window_2d);
            SDL_RaiseWindow(window_main->window);
            window_2d = NULL;
            show_2d = false;
          }
        }
      }
      if (event.type == SDL_KEYUP)
      {
        key_pressed = false;
      }
    }

    renderScene(window_main->renderer, *scene, renderFpScene);
    updateFrameCounter(window_main);

    if (show_2d)
    {
      renderScene(window_2d->renderer, *scene, render2dScene);
    }

    processPlayerMotion(&scene->player, 1 / window_main->fps, scene->map);
  }

  // Cleanup and exit
  freeScene(scene);
  freeWindowData(window_main);
  freeWindowData(window_2d);
  IMG_Quit();
  SDL_Quit();
  return 0;
}
