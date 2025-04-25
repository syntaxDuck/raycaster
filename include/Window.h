#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL_render.h>
#include <SDL.h>

typedef struct
{
  char *title;
  int width;
  int height;
  int max_fps;
} WindowConfig;

typedef struct
{
  bool quit;
  bool key_pressed;
  int frame_count;
  float fps;
  Uint32 last_time;
} WindowState;

typedef struct
{
  WindowConfig *ptr_config;
  SDL_Window *ptr_window;
  SDL_Renderer *ptr_renderer;
  WindowState state;
} WindowCtx;

WindowCtx *init_window(WindowConfig *ptr_config);
void handle_window_events(WindowCtx *ptr_ctx, SDL_Event event);
void update_frame_counter(WindowCtx *ptr_ctx);
void free_window_ctx(WindowCtx *ptr_ctx);

#endif
