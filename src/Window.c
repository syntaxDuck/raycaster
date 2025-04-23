#include "window.h"
#include <stdio.h>

WindowCtx *window_init(char *title, int x, int y, int width, int height,
                       int target_fps)
{
  // Create the window
  WindowCtx *ctx = malloc(sizeof(WindowCtx));
  ctx->window = SDL_CreateWindow(title, x, y, width, height,
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

  // Initialize frame count and copy title
  ctx->title = malloc(strlen(title) + 1); // +1 for null terminator
  if (ctx->title == NULL)
  {
    fprintf(stderr, "Memory allocation for window title failed!\n");
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
    SDL_Quit();
    return NULL;
  }
  strcpy(ctx->title, title);

  ctx->frame_count = 0;
  ctx->fps = target_fps;
  ctx->last_time = SDL_GetTicks();
  ctx->state.quit = false;
  ctx->width = width;
  ctx->height = height;

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
      SDL_GetWindowSizeInPixels(ctx->window, &ctx->width, &ctx->height);
    }
  }
}

void update_frame_counter(WindowCtx *window_data)
{
  // Increment the frame count
  window_data->frame_count++;

  // Calculate time elapsed since the last FPS update
  Uint32 currentTime = SDL_GetTicks();
  Uint32 timeElapsed = currentTime - window_data->last_time; // In milliseconds

  // Update FPS once per second (1000 milliseconds)
  if (timeElapsed >= 1000)
  {
    // Calculate the frames per second (FPS)
    window_data->fps =
        (float)window_data->frame_count / (timeElapsed / 1000.0f);

    // Reset frame count and lastTime for the next FPS calculation
    window_data->frame_count = 0;
    window_data->last_time = currentTime;

    // Create a new title string that includes the FPS count
    char title[256];
    snprintf(title, sizeof(title), "%s - FPS: %.2f", window_data->title,
             window_data->fps);

    // Set the new window title with the FPS
    SDL_SetWindowTitle(window_data->window, title);
  }
}

void free_window_ctx(WindowCtx *ctx)
{
  if (ctx->renderer)
    SDL_DestroyRenderer(ctx->renderer);
  if (ctx->window)
    SDL_DestroyWindow(ctx->window);
  free(ctx->title);
}
