#include "Window.h"

WindowCtx *createWindow(char *title, int x, int y, int width, int height)
{
    // Create the window
    WindowCtx *window = malloc(sizeof(WindowCtx));
    window->window = SDL_CreateWindow(title,
                                      x, y,
                                      width, height,
                                      SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (window->window == NULL)
    {
        fprintf(stderr,
                "Window could not be created! SDL_Error: %s\n",
                SDL_GetError());
        SDL_Quit();
        return false;
    }

    // Create the renderer
    window->renderer = SDL_CreateRenderer(window->window,
                                          -1,
                                          SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    if (window->renderer == NULL)
    {
        fprintf(stderr,
                "Renderer could not be created! SDL_Error: %s\n",
                SDL_GetError());
        SDL_DestroyWindow(window->window);
        SDL_Quit();
        return false;
    }

    // Set the renderer color (optional)
    SDL_SetRenderDrawBlendMode(window->renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(window->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // Initialize frame count and copy title
    window->title = malloc(strlen(title) + 1); // +1 for null terminator
    if (window->title == NULL)
    {
        fprintf(stderr, "Memory allocation for window title failed!\n");
        SDL_DestroyRenderer(window->renderer);
        SDL_DestroyWindow(window->window);
        SDL_Quit();
        return false;
    }
    strcpy(window->title, title);

    window->frame_count = 0;
    window->fps = MAX_FPS;
    window->last_time = SDL_GetTicks();
    window->state.quit = false;

    return window;
}

void handleWindowEvents(WindowState *state, SDL_Event event)
{
    if (event.type == SDL_QUIT)
    {
        state->quit = true;
    }

    if (event.type == SDL_WINDOWEVENT)
    {
        // if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        // {
        //     game_ctx->game_vp.w = event.window.data1 - game_ctx->game_vp.x;
        //     game_ctx->game_vp.h = event.window.data2 - game_ctx->game_vp.y;

        //     editor_ctx->menu_vp.h = event.window.data2;
        // }
    }
}

void updateFrameCounter(WindowCtx *window_data)
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
        window_data->fps = (float)window_data->frame_count / (timeElapsed / 1000.0f);

        // Reset frame count and lastTime for the next FPS calculation
        window_data->frame_count = 0;
        window_data->last_time = currentTime;

        // Create a new title string that includes the FPS count
        char title[256];
        snprintf(title,
                 sizeof(title),
                 "%s - FPS: %.2f",
                 window_data->title,
                 window_data->fps);

        // Set the new window title with the FPS
        SDL_SetWindowTitle(window_data->window, title);
    }
}

void freeWindowCtx(WindowCtx *window_data)
{
    SDL_DestroyRenderer(window_data->renderer);
    SDL_DestroyWindow(window_data->window);
    free(window_data->title);
    free(window_data);
}