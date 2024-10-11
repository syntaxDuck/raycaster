#ifndef EDITOR_H
#define EDITOR_H

#include <stdbool.h>
#include "Window.h"

#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear.h"
#include "nuklear_sdl_renderer.h"

typedef struct
{
    bool quit;
    bool show_2d;
    bool game_focused;
    bool key_pressed;
} EditorEventCtx;

typedef struct
{
    struct nk_context *menu_ctx;
    EditorEventCtx event_ctx;
    struct nk_rect menu_vp;
} EditorCtx;

// Function declarations
void render_nuklear(struct nk_context *ctx, struct nk_rect vp,
                    SDL_Renderer *renderer);

void handleWindowEvents(EditorCtx *editor_ctx, WindowCtx *window_main);

struct nk_context *setupMenu(WindowCtx *window);
#endif