#include "window.h"
#include "config.h"
#include <stdio.h>

WindowCtx *init_window(WindowConfig *config)
{
  // Create the window
  WindowCtx *ctx = malloc(sizeof(WindowCtx));
  ctx->config = config;
  ctx->window = SDL_CreateWindow(config->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 config->width, config->height,
                                 SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (ctx->window == NULL)
  {
    fprintf(stderr, "Window could not be created! SDL_Error: %s\n",
            SDL_GetError());
    SDL_Quit();
    return NULL;
  }

  // Create the renderer
  ctx->renderer = SDL_CreateRenderer(
      ctx->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ctx->renderer == NULL)
  {
    fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n",
            SDL_GetError());
    SDL_DestroyWindow(ctx->window);
    SDL_Quit();
    return NULL;
  }

  // Set the renderer color (optional)
  SDL_SetRenderDrawBlendMode(ctx->renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(ctx->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  ctx->state.frame_count = 0;
  ctx->state.fps = config->max_fps;
  ctx->state.last_time = SDL_GetTicks();
  ctx->state.quit = false;

  return ctx;
}

void handle_window_events(WindowCtx *ctx, SDL_Event event)
{
  WindowConfig *config = ctx->config;
  if (event.type == SDL_QUIT)
  {
    ctx->state.quit = true;
  }

  if (event.type == SDL_WINDOWEVENT)
  {
    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
      SDL_GetWindowSizeInPixels(ctx->window, &config->width, &config->height);
    }
  }
}

void update_frame_counter(WindowCtx *ctx)
{
  // Increment the frame count
  ctx->state.frame_count++;

  // Calculate time elapsed since the last FPS update
  Uint32 currentTime = SDL_GetTicks();
  Uint32 timeElapsed = currentTime - ctx->state.last_time; // In milliseconds

  // Update FPS once per second (1000 milliseconds)
  if (timeElapsed >= 1000)
  {
    // Calculate the frames per second (FPS)
    ctx->state.fps =
        (float)ctx->state.frame_count / (timeElapsed / 1000.0f);

    // Reset frame count and lastTime for the next FPS calculation
    ctx->state.frame_count = 0;
    ctx->state.last_time = currentTime;

    // Create a new title string that includes the FPS count
    char title[256];
    snprintf(title, sizeof(title), "%s - FPS: %.2f", ctx->config->title,
             ctx->state.fps);

    // Set the new window title with the FPS
    SDL_SetWindowTitle(ctx->window, title);
  }
}

void free_window_ctx(WindowCtx *ctx)
{
  if (ctx->renderer)
    SDL_DestroyRenderer(ctx->renderer);
  if (ctx->window)
    SDL_DestroyWindow(ctx->window);
  free(ctx->config->title);
}
