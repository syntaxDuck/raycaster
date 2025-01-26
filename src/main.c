#include "config.h"
#include "global.h"

#include "game.h"
#include "scene.h"
#include "window.h"

int initSDL() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n",
            SDL_GetError());
    return 1;
  }

  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) {
    fprintf(stderr, "IMG_Init Error: %s\n", IMG_GetError());
    SDL_Quit(); // Clean up SDL
    return 1;   // or handle the error appropriately
  }
  return 0;
}

int main(int argc, char *argv[]) {
  initSDL();
  win_ctx = windowInit("Main Viewport", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, MAX_FPS);

  GameCtx game_ctx;

  game_ctx.scene = createScene("./assets/maps/map.txt");

  SDL_Event event;
  while (!win_ctx->state.quit) {
    if (SDL_PollEvent(&event) != 0) {
      handleWindowEvents(win_ctx, event);
    }

    renderScene(*game_ctx.scene, renderFpScene);
    SDL_RenderPresent(win_ctx->renderer);
    updateFrameCounter(win_ctx);
    processPlayerMotion(&game_ctx.scene->player, win_ctx->fps,
                        game_ctx.scene->map);
  }

  freeScene(game_ctx.scene);
  freeWindowCtx(win_ctx);
  IMG_Quit();
  SDL_Quit();
  return 0;
}
