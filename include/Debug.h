#ifndef EDITOR_H
#define EDITOR_H

#include <stdbool.h>

#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "nuklear.h"
#include "nuklear_sdl_renderer.h"
#include "Config.h"
#include "Global.h"
#include "Window.h"

typedef struct
{
    bool show_2d;
    bool focused;
    bool key_pressed;
} DebugState;

typedef struct
{
    struct nk_context *menu_ctx;
    DebugState state;
} DebugCtx;

void handleDebugEvents(DebugState *state, SDL_Event event);

// Function declarations
void render_nuklear(struct nk_context *ctx, SDL_Renderer *renderer);

struct nk_context *setupMenu(WindowCtx *ctx);
#endif