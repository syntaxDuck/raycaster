#include "config.h"
#include "global.h"

#include "Debug.h"
#include "Game.h"
#include "Scene.h"
#include "Texture.h"
#include "window.h"

#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl_renderer.h"

SDL_Rect createRect(int x, int y, int w, int h)
{
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  return rect;
}

int initSDL()
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr,
            "SDL could not initialize! SDL_Error: %s\n",
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
  initSDL();
  win_ctx = windowInit("Main Viewport",
                       SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED,
                       WIN_WIDTH, WIN_HEIGHT, MAX_FPS);

  DebugCtx debug_ctx;
  GameCtx game_ctx;
#ifdef DEBUG
  debug_ctx.menu_ctx = setupMenu(win_ctx);
  debug_ctx.state.show_2d = false;
  debug_ctx.state.focused = false;
  debug_ctx.state.key_pressed = false;
#endif

  game_ctx.scene = createScene("./assets/maps/map.txt");

  SDL_Event event;
  while (!win_ctx->state.quit)
  {
    if (SDL_PollEvent(&event) != 0)
    {
      handleWindowEvents(win_ctx, event);

#ifdef DEBUG
      nk_input_begin(debug_ctx.menu_ctx);
      handleDebugEvents(&debug_ctx.state, event);
      nk_sdl_handle_event(&event);
      nk_input_end(debug_ctx.menu_ctx);
#endif
    }

    renderScene(*game_ctx.scene, renderFpScene);

#ifdef DEBUG
    render_nuklear(debug_ctx.menu_ctx,
                   win_ctx->renderer);
#endif

    SDL_RenderPresent(win_ctx->renderer);
    updateFrameCounter(win_ctx);

    processPlayerMotion(&game_ctx.scene->player,
                        win_ctx->fps,
                        game_ctx.scene->map);
  }

// Cleanup and exit
#ifdef DEBUG
  nk_sdl_shutdown();
#endif

  freeScene(game_ctx.scene);
  freeWindowCtx(win_ctx);
  IMG_Quit();
  SDL_Quit();
  return 0;
}
