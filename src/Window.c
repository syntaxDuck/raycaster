#include "window.h"
#include "config.h"
#include <stdio.h>

WindowCtx *init_window(WindowConfig *window_config)
{
  // Create the window
  WindowCtx *ctx = malloc(sizeof(WindowCtx));
  ctx->window_config = window_config;
  ctx->window = SDL_CreateWindow(window_config->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 window_config->width, window_config->height,
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
  ctx->state.fps = window_config->max_fps;
  ctx->state.last_time = SDL_GetTicks();
  ctx->state.quit = false;

  return ctx;
}

void handle_window_events(WindowCtx *ctx, SDL_Event event)
{
  if (event.type == SDL_QUIT)
  {
    ctx->state.quit = true;
  }

  if (event.type == SDL_WINDOWEVENT)
  {
    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
      SDL_GetWindowSizeInPixels(ctx->window,
                                &ctx->window_config->width,
                                &ctx->window_config->height);
    }
  }
}

void update_frame_counter(WindowCtx *ctx)
{
  ctx->state.frame_count++;

  uint32_t current_time = SDL_GetTicks();
  uint32_t time_elapsed = current_time - ctx->state.last_time; // In milliseconds

  if (time_elapsed >= 1000)
  {
    ctx->state.fps =
        (float)ctx->state.frame_count / (time_elapsed / 1000.0f);

    ctx->state.frame_count = 0;
    ctx->state.last_time = current_time;

    char formatted_title[256];
    snprintf(formatted_title, sizeof(formatted_title), "%s - FPS: %.2f", ctx->window_config->title,
             ctx->state.fps);

    SDL_SetWindowTitle(ctx->window, formatted_title);
  }
}

void free_window_ctx(WindowCtx *ctx)
{
  if (ctx->renderer)
    SDL_DestroyRenderer(ctx->renderer);
  if (ctx->window)
    SDL_DestroyWindow(ctx->window);
  free(ctx->window_config->title);
}
