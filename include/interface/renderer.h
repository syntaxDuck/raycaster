#ifndef _RENDERER_H
#define _RENDERER_H

#include "sdl_window.h"

typedef struct Renderer Renderer;

struct Texture;
typedef struct Texture Texture;

typedef struct
{
    int x, y;
    int w, h;
} Rect;

Renderer *create_renderer(Window *win);
Texture *create_texture(Renderer *rend, uint32_t format, uint8_t access_type,
                        int width, int height);
void render_copy(Renderer *rend, Texture *tex, Rect *rect1, Rect *rect2);
void lock_texture(Texture *tex, Rect *rect, void **pixels, int *pitch);
void unlock_texture(Texture *tex);
void destroy_texture(Texture *tex);
void set_texture_blend_mode(Texture *tex, int mode);
void set_render_draw_color(Renderer *rend, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void render_fill_rect(Renderer *rend, Rect *rect);
void render_draw_line(Renderer *rend, int x1, int y1, int x2, int y2);
void clear_renderer(Renderer *rend);
void render_present(Renderer *rend);
void free_renderer(Renderer *rend);
#endif