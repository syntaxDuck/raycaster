#include "window_ctx.h"

WindowCtx *create_window_ctx(WindowConfig *config)
{
    WindowCtx *ctx = malloc(sizeof(WindowCtx));
    ctx->window_config = config;

    ctx->window = create_window(config->title, config->width, config->height);
    ctx->renderer = create_renderer(ctx->window);
    initialize_window_context(ctx);

    return ctx;
}

static int initialize_window_context(WindowCtx *ctx)
{
    ctx->state.frame_count = 0;
    ctx->state.fps = ctx->window_config->max_fps;
    ctx->state.last_time = SDL_GetTicks();
    ctx->state.quit = false;
    return 0;
}

void free_window_ctx(WindowCtx *ctx)
{
    free_window(ctx->window);
    free(ctx->window_config->title);
}