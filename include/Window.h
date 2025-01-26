#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  bool quit;
  bool key_pressed;
} WindowState;

typedef struct {
  char *title;
  SDL_Window *window;
  SDL_Renderer *renderer;
  WindowState state;
  Uint32 last_time;
  int frame_count;
  float fps;
  int width;
  int height;
} WindowCtx;

WindowCtx *window_init(char *title, int x, int y, int width, int height,
                       int target_fps);
void handle_window_events(WindowCtx *ctx, SDL_Event event);
void update_frame_counter(WindowCtx *ctx);
void free_window_ctx(WindowCtx *ctx);
#endif
