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
  uint32_t last_time;
} WindowState;

typedef struct
{
  WindowConfig *window_config;
  SDL_Window *window;
  SDL_Renderer *renderer;
  WindowState state;
  void (*render_function)();
} WindowCtx;

WindowCtx *create_window_ctx(WindowConfig *config);
void handle_window_events(WindowCtx *ctx, SDL_Event event);
void render_window(WindowCtx *ctx);
void free_window_ctx(WindowCtx *ctx);

#endif
