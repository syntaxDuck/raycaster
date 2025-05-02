#include "scene.h"
#include "window_ctx.h"
#include "config.h"
#include <stdio.h>
#include <SDL_image.h>

int initialize_SDL()
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n",
            SDL_GetError());
    return 1;
  }

  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
  {
    fprintf(stderr, "IMG_Init Error: %s\n", IMG_GetError());
    SDL_Quit(); // Clean up SDL
    return 1;   // or handle the error appropriately
  }
  return 0;
}

void release_SDL_resources()
{
  IMG_Quit();
  SDL_Quit();
}

int main(int argc, char *argv[])
{
  initialize_SDL();
  Config *config = init_config();
  WindowCtx *window_ctx = create_window_ctx(config->ptr_window_config);
  Scene *scene = create_scene(window_ctx, "assets/maps/map.txt");
  set_current_scene(scene);
  window_ctx->render_function = render_fp_scene;

  SDL_Event event;
  while (!window_ctx->state.quit)
  {
    // if (SDL_PollEvent(&event) != 0)
    // {
    //   handle_window_events(window_ctx, event);
    // }

    window_ctx->render_function();
    render_present(window_ctx->renderer);

    process_player_motion(&scene->player, window_ctx->state.fps,
                          scene->map);
  }

  free_scene(scene);
  free_window_ctx(window_ctx);
  release_SDL_resources();
  return 0;
}
