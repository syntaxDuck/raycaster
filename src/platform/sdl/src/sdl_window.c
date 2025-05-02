#include "window.h"

#include <SDL.h>
#include <stdio.h>

struct Window
{
  SDL_Window *sdl_window;
};

Window *create_window(char *title, int width, int height)
{
  Window *win = malloc(sizeof(Window));
  win->sdl_window = SDL_CreateWindow(title,
                                     SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     width, height,
                                     SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (win->sdl_window == NULL)
  {
    fprintf(stderr, "Window could not be created! SDL_Error: %s\n",
            SDL_GetError());
    return NULL;
  }

  return win;
}

// static void update_frame_counter(WindowCtx *ctx)
// {
//   ctx->state.frame_count++;

//   uint32_t current_time = SDL_GetTicks();
//   uint32_t time_elapsed = current_time - ctx->state.last_time; // In milliseconds

//   if (time_elapsed >= 1000)
//   {
//     ctx->state.fps =
//         (float)ctx->state.frame_count / (time_elapsed / 1000.0f);

//     ctx->state.frame_count = 0;
//     ctx->state.last_time = current_time;

//     char formatted_title[256];
//     snprintf(formatted_title, sizeof(formatted_title), "%s - FPS: %.2f", ctx->window_config->title,
//              ctx->state.fps);

//     SDL_SetWindowTitle(ctx->window, formatted_title);
//   }
// }

// void handle_window_events(WindowCtx *ctx, SDL_Event event)
// {
//   if (event.type == SDL_QUIT)
//   {
//     ctx->state.quit = true;
//   }

//   if (event.type == SDL_WINDOWEVENT)
//   {
//     if (event.window.event == SDL_WINDOWEVENT_RESIZED)
//     {
//       SDL_GetWindowSizeInPixels(ctx->window,
//                                 &ctx->window_config->width,
//                                 &ctx->window_config->height);
//     }
//   }
// }

void get_window_size_in_pixels(Window *win, int *w, int *h)
{
  SDL_GetWindowSizeInPixels(win->sdl_window, w, h);
}

void free_window(Window *win)
{
  if (win->sdl_window)
    SDL_DestroyWindow(win->sdl_window);
}
