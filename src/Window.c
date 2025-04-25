#include "window.h"
#include "config.h"
#include <stdio.h>

WindowCtx *init_window(WindowConfig *ptr_config)
{
  // Create the ptr_window
  WindowCtx *ptr_ctx = malloc(sizeof(WindowCtx));
  ptr_ctx->ptr_config = ptr_config;
  ptr_ctx->ptr_window = SDL_CreateWindow(ptr_config->title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                         ptr_config->width, ptr_config->height,
                                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (ptr_ctx->ptr_window == NULL)
  {
    fprintf(stderr, "Window could not be created! SDL_Error: %s\n",
            SDL_GetError());
    SDL_Quit();
    return NULL;
  }

  // Create the ptr_renderer
  ptr_ctx->ptr_renderer = SDL_CreateRenderer(
      ptr_ctx->ptr_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (ptr_ctx->ptr_renderer == NULL)
  {
    fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n",
            SDL_GetError());
    SDL_DestroyWindow(ptr_ctx->ptr_window);
    SDL_Quit();
    return NULL;
  }

  // Set the ptr_renderer color (optional)
  SDL_SetRenderDrawBlendMode(ptr_ctx->ptr_renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(ptr_ctx->ptr_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  ptr_ctx->state.frame_count = 0;
  ptr_ctx->state.fps = ptr_config->max_fps;
  ptr_ctx->state.last_time = SDL_GetTicks();
  ptr_ctx->state.quit = false;

  return ptr_ctx;
}

void handle_window_events(WindowCtx *ptr_ctx, SDL_Event event)
{
  if (event.type == SDL_QUIT)
  {
    ptr_ctx->state.quit = true;
  }

  if (event.type == SDL_WINDOWEVENT)
  {
    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
    {
      SDL_GetWindowSizeInPixels(ptr_ctx->ptr_window,
                                &ptr_ctx->ptr_config->width,
                                &ptr_ctx->ptr_config->height);
    }
  }
}

void update_frame_counter(WindowCtx *ptr_ctx)
{
  ptr_ctx->state.frame_count++;

  Uint32 current_time = SDL_GetTicks();
  Uint32 time_elapsed = current_time - ptr_ctx->state.last_time; // In milliseconds

  if (time_elapsed >= 1000)
  {
    ptr_ctx->state.fps =
        (float)ptr_ctx->state.frame_count / (time_elapsed / 1000.0f);

    ptr_ctx->state.frame_count = 0;
    ptr_ctx->state.last_time = current_time;

    char formatted_title[256];
    snprintf(formatted_title, sizeof(formatted_title), "%s - FPS: %.2f", ptr_ctx->ptr_config->title,
             ptr_ctx->state.fps);

    SDL_SetWindowTitle(ptr_ctx->ptr_window, formatted_title);
  }
}

void free_window_ctx(WindowCtx *ptr_ctx)
{
  if (ptr_ctx->ptr_renderer)
    SDL_DestroyRenderer(ptr_ctx->ptr_renderer);
  if (ptr_ctx->ptr_window)
    SDL_DestroyWindow(ptr_ctx->ptr_window);
  free(ptr_ctx->ptr_config->title);
}
