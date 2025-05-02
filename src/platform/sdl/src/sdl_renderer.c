#include "renderer.h"
#include "sdl_window.h"

#include <SDL.h>
#include <stdio.h>

struct Renderer
{
    SDL_Renderer *sdl_renderer;
};

Renderer *create_renderer(Window *win)
{
    Renderer *rend = malloc(sizeof(Renderer));
    rend->sdl_renderer = SDL_CreateRenderer(
        win->sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (rend == NULL)
    {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n",
                SDL_GetError());
        SDL_DestroyWindow(win->sdl_window);
        return NULL;
    }

    SDL_SetRenderDrawBlendMode(rend->sdl_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rend->sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    return rend;
}

void render_present(Renderer *rend)
{
    // ctx->render_function();
    // if (ctx->window_config->show_fps)
    //   update_frame_counter(ctx);
    SDL_RenderPresent(rend->sdl_renderer);
}

void set_render_draw_color(Renderer *rend, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(rend->sdl_renderer, r, g, b, a);
}

void clear_renderer(Renderer *rend)
{
    SDL_RenderClear(rend->sdl_renderer);
}

void render_fill_rect(Renderer *rend, Rect *rect)
{
    SDL_RenderFillRect(rend->sdl_renderer, (SDL_Rect *)rect);
}

void render_draw_line(Renderer *rend, int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(rend->sdl_renderer, x1, y1, x2, y2);
}

Texture *create_texture(Renderer *rend, uint32_t format, uint8_t access_type, 
    int width, int height)
{
    SDL_Texture *texture = SDL_CreateTexture(
        rend->sdl_renderer, format, access_type,
        width, height);
    return (Texture *)texture;
}

void set_texture_blend_mode(Texture *tex, int mode)
{
    SDL_SetTextureBlendMode((SDL_Texture *)tex, mode);
}

void lock_texture(Texture *tex, Rect *rect, void **pixels, int *pitch)
{
    SDL_LockTexture((SDL_Texture *)tex, NULL, pixels, pitch);
}
void unlock_texture(Texture *tex)
{
    SDL_UnlockTexture((SDL_Texture *)tex);
}
void destroy_texture(Texture *tex)
{
    SDL_DestroyTexture((SDL_Texture *)tex);
}

void render_copy(Renderer *rend, Texture *tex, Rect *rect1, Rect *rect2)
{
    SDL_RenderCopy(rend->sdl_renderer, (SDL_Texture *)tex, (SDL_Rect *)rect1, (SDL_Rect *)rect2);
}

void free_renderer(Renderer *rend)
{
    SDL_DestroyRenderer(rend->sdl_renderer);
    free(rend);
}