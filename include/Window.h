#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdint.h>

#include "Scene.h"

typedef struct
{
    char *title;
    SDL_Window *window;
    SDL_Renderer *renderer;
    int frame_count;
    float fps;
    Uint32 last_time;
} WindowData;

WindowData *createWindow(char *title, int x, int y, int width, int height);
void renderScene(SDL_Renderer *renderer, Scene scene, void (*render)(Scene, SDL_Renderer *renderer));
void updateFrameCounter(WindowData *window_data);
void freeWindowData(WindowData *window_data);
#endif