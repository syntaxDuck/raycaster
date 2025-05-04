#include "renderer.h"

#include <SDL.h>
#include <stdio.h>

Renderer *create_renderer(Window *win)
{
    SDL_Renderer *rend = SDL_CreateRenderer(
        (SDL_Window*)win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (rend == NULL)
    {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n",
                SDL_GetError());
        SDL_DestroyWindow((SDL_Window *)win);
        return NULL;
    }

    SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(rend, 0xFF, 0xFF, 0xFF, 0xFF);

    return (Renderer *)rend;
}

void render_present(Renderer *rend)
{
    // ctx->render_function();
    // if (ctx->window_config->show_fps)
    //   update_frame_counter(ctx);
    SDL_RenderPresent((SDL_Renderer *)rend);
}

void set_render_draw_color(Renderer *rend, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor((SDL_Renderer *)rend, r, g, b, a);
}

void clear_renderer(Renderer *rend)
{
    SDL_RenderClear((SDL_Renderer *)rend);
}

void render_fill_rect(Renderer *rend, Rect *rect)
{
    SDL_RenderFillRect((SDL_Renderer *)rend, (SDL_Rect *)rect);
}

void render_draw_line(Renderer *rend, int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine((SDL_Renderer *)rend, x1, y1, x2, y2);
}

Texture *create_texture(Renderer *rend, uint32_t format, uint8_t access_type,
                        int width, int height)
{
    SDL_Texture *texture = SDL_CreateTexture(
        (SDL_Renderer *)rend, format, access_type,
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
    SDL_RenderCopy((SDL_Renderer *)rend, (SDL_Texture *)tex, (SDL_Rect *)rect1, (SDL_Rect *)rect2);
}

void free_renderer(Renderer *rend)
{
    SDL_DestroyRenderer((SDL_Renderer *)rend);
    free(rend);
}