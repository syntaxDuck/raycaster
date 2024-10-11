#include "Editor.h"

// TODO: Make system compatable with resizing window
void render_nuklear(struct nk_context *ctx, struct nk_rect vp,
                    SDL_Renderer *renderer)
{
    if (nk_begin(ctx, "Editor Menu", vp,
                 NK_WINDOW_BORDER |
                     NK_WINDOW_SCALABLE |
                     NK_WINDOW_TITLE))
    {
        struct nk_vec2 window_size = nk_window_get_size(ctx);
        nk_window_set_size(ctx, "Demo", nk_vec2(window_size.x, vp.h));
        static char text[64] = "Hello, Nuklear!";
        nk_layout_row_static(ctx, 30, 80, 1);
        if (nk_button_label(ctx, "Button"))
        {
            printf("Button pressed!\n");
        }

        nk_layout_row_dynamic(ctx, 25, 1);
        nk_label(ctx, "This is a label", NK_TEXT_LEFT);

        nk_layout_row_dynamic(ctx, 30, 1);
        nk_edit_string_zero_terminated(ctx, NK_EDIT_SIMPLE, text,
                                       sizeof(text), nk_filter_default);
    }
    nk_end(ctx);

    // Render Nuklear UI
    nk_sdl_render(NK_ANTI_ALIASING_ON);
}

// TODO: this function seems to be throwing an exception when multiple
//       keys are pressed
void handleWindowEvents(EditorCtx *editor_ctx, WindowCtx *window_main)
{
    // Handle events
    EditorEventCtx event_ctx = editor_ctx->event_ctx;
    SDL_Event event;
#ifdef DEBUG
    nk_input_begin(editor_ctx->menu_ctx);
#endif

    while (SDL_PollEvent(&event) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            event_ctx.quit = true;
        }
#ifdef DEBUG
        if (event.type == SDL_KEYDOWN && !event_ctx.key_pressed)
        {
            event_ctx.key_pressed = true;
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            if (state[SDL_SCANCODE_2])
            {
                if (!event_ctx.show_2d)
                {
                    event_ctx.show_2d = true;
                }
                else
                {
                    event_ctx.show_2d = false;
                }
            }
        }
        if (event.type == SDL_KEYUP)
        {
            event_ctx.key_pressed = false;
        }

        if (event.type == SDL_MOUSEMOTION && event.motion.x < WIN_WIDTH / 2)
        {
            event_ctx.game_focused = false;
        }

        if (event.type == SDL_MOUSEMOTION && event.motion.x >= WIN_WIDTH / 2)
        {
            event_ctx.game_focused = true;
        }
        nk_sdl_handle_event(&event);
#endif
    }

#ifdef DEBUG
    nk_input_end(editor_ctx->menu_ctx);
#endif
}

struct nk_context *setupMenu(WindowCtx *window)
{
    struct nk_context *ctx = nk_sdl_init(window->window,
                                         window->renderer);
    struct nk_font_atlas *atlas;
    struct nk_font_config config = nk_font_config(0);
    struct nk_font *font;

    float font_scale = 1;
    nk_sdl_font_stash_begin(&atlas);
    font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
    nk_sdl_font_stash_end();

    font->handle.height /= font_scale;
    nk_style_set_font(ctx, &font->handle);

    return ctx;
}