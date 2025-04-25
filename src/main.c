#include "game.h"
#include "scene.h"
#include "window.h"
#include "config.h"
#include <stdio.h>

int init_SDL()
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

int main(int argc, char *argv[])
{
  init_SDL();
  Config *window_config = init_config();
  WindowCtx *window_ctx = init_window(window_config->ptr_window_config);
  Scene *scene = createScene("assets/maps/map.txt");

  // GameCtx game_ctx;

  // game_ctx.scene = createScene("assets/maps/map.txt");

  // SDL_Event event;
  // while (!window_ctx->state.quit)
  // {
  //   if (SDL_PollEvent(&event) != 0)
  //   {
  //     handle_window_events(window_ctx, event);
  //   }

  //   render_scene(*game_ctx.scene, render_fp_scene);
  //   SDL_RenderPresent(window_ctx->renderer);
  //   update_frame_counter(window_ctx);
  //   process_player_motion(&game_ctx.scene->player, window_ctx->fps,
  //                         game_ctx.scene->map);
  // }

  // freeScene(game_ctx.scene);
  free_window_ctx(window_ctx);
  IMG_Quit();
  SDL_Quit();
  return 0;
}
