#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdint.h>
#include <stdbool.h>

#include "Config.h"

typedef struct
{
    bool quit;
    bool key_pressed;
} WindowState;

typedef struct
{
    char *title;
    SDL_Window *window;
    SDL_Renderer *renderer;
    WindowState state;
    Uint32 last_time;
    int frame_count;
    float fps;
    int width, height;
} WindowCtx;

WindowCtx *createWindow(char *title, int x, int y, int width, int height);
void handleWindowEvents(WindowCtx *ctx, SDL_Event event);
void updateFrameCounter(WindowCtx *window_data);
void freeWindowCtx(WindowCtx *window_data);
#endif