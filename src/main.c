#include "Defines.h"
#include "Editor.h"
#include "Scene.h"
#include "Texture.h"
#include "Window.h"

#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl_renderer.h"

typedef struct
{
  Scene *scene;
  SDL_Rect game_vp;
} GameCtx;

SDL_Rect createRect(int x, int y, int w, int h)
{
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  return rect;
}

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

  WindowCtx *window_main = createWindow("Main Viewport",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WIN_WIDTH, WIN_HEIGHT);

  EditorCtx editor_ctx;
  GameCtx game_ctx;
#ifdef DEBUG
  editor_ctx.menu_ctx = setupMenu(window_main);
  editor_ctx.menu_vp = nk_rect(0, 0, WIN_WIDTH / 3, WIN_HEIGHT);

  game_ctx.game_vp = createRect(editor_ctx.menu_vp.x + editor_ctx.menu_vp.w,
                                editor_ctx.menu_vp.y,
                                WIN_WIDTH - editor_ctx.menu_vp.w,
                                WIN_HEIGHT);
#else
  game_ctx.game_vp = createRect(0, 0, WIN_WIDTH, WIN_HEIGHT);
#endif

  // Create the scene (map and player)
  game_ctx.scene = createScene();

  // Main game loop
  EditorEventCtx event_ctx;
  event_ctx.quit = false;
  event_ctx.show_2d = false;
  event_ctx.game_focused = false;
  event_ctx.key_pressed = false;

  editor_ctx.event_ctx = event_ctx;

  SDL_Event event;
  while (!editor_ctx.event_ctx.quit)
  {
    handleWindowEvents(&editor_ctx, window_main);

    SDL_RenderSetViewport(window_main->renderer, &game_ctx.game_vp);
    renderScene(window_main->renderer, *game_ctx.scene, renderFpScene);

#ifdef DEBUG
    SDL_Rect vp;
    vp.x = editor_ctx.menu_vp.x;
    vp.y = editor_ctx.menu_vp.y;
    vp.w = editor_ctx.menu_vp.w;
    vp.h = editor_ctx.menu_vp.h;
    SDL_RenderSetViewport(window_main->renderer, &vp);
    render_nuklear(editor_ctx.menu_ctx,
                   editor_ctx.menu_vp,
                   window_main->renderer);
#endif

    SDL_RenderPresent(window_main->renderer);
    updateFrameCounter(window_main);
    processPlayerMotion(&game_ctx.scene->player,
                        1 / window_main->fps,
                        game_ctx.scene->map);
  }

// Cleanup and exit
#ifdef DEBUG
  nk_sdl_shutdown();
#endif

  freeScene(game_ctx.scene);
  freeWindowData(window_main);
  IMG_Quit();
  SDL_Quit();
  return 0;
}
