#include "Defines.h"
#include "Scene.h"
#include "Texture.h"
#include "Window.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION
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
  struct nk_context *nk_ctx;
  EditorEventCtx *event_ctx;
  SDL_Rect menu_vp;
  SDL_Rect game_vp;
} EditorCtx;

// TODO: Make system compatable with resizing window
void render_nuklear(struct nk_context *ctx, struct nk_rect vp, SDL_Renderer *renderer)
{
  if (nk_begin(ctx, "Demo", vp,
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
void handleWindowEvents(struct nk_context *ctx, EditorEventCtx *event_ctx,
                        WindowCtx *window_main)
{
  // Handle events
  SDL_Event event;
#ifdef DEBUG
  nk_input_begin(ctx);
#endif

  while (SDL_PollEvent(&event) != 0)
  {
    if (event.type == SDL_QUIT)
    {
      event_ctx->quit = true;
    }
#ifdef DEBUG
    if (event.type == SDL_KEYDOWN && !event_ctx->key_pressed)
    {
      event_ctx->key_pressed = true;
      const Uint8 *state = SDL_GetKeyboardState(NULL);
      if (state[SDL_SCANCODE_2])
      {
        if (!event_ctx->show_2d)
        {
          event_ctx->show_2d = true;
        }
        else
        {
          event_ctx->show_2d = false;
        }
      }
    }
    if (event.type == SDL_KEYUP)
    {
      event_ctx->key_pressed = false;
    }

    if (event.type == SDL_MOUSEMOTION && event.motion.x < WIN_WIDTH / 2)
    {
      event_ctx->game_focused = false;
    }

    if (event.type == SDL_MOUSEMOTION && event.motion.x >= WIN_WIDTH / 2)
    {
      event_ctx->game_focused = true;
    }
    nk_sdl_handle_event(&event);
#endif
  }

#ifdef DEBUG
  nk_input_end(ctx);
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

SDL_Rect *createRect(int x, int y, int w, int h)
{
  SDL_Rect *rect = malloc(sizeof(SDL_Rect));
  rect->x = x;
  rect->y = y;
  rect->w = w;
  rect->h = h;
  return rect;
}

int main(int argc, char *argv[])
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr,
            "SDL could not initialize! SDL_Error: %s\n",
            SDL_GetError());
    return -1;
  }

  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
  {
    fprintf(stderr, "IMG_Init Error: %s\n", IMG_GetError());
    SDL_Quit(); // Clean up SDL
    return 1;   // or handle the error appropriately
  }

  WindowCtx *window_main = createWindow("Main Viewport",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WIN_WIDTH, WIN_HEIGHT);

  struct nk_context *ctx;
#ifdef DEBUG
  EditorCtx editor_ctx;
  ctx = setupMenu(window_main);

  struct nk_rect nk_vp = nk_rect(0, 0, WIN_WIDTH / 3, WIN_HEIGHT);
  SDL_Rect *game_vp = createRect(nk_vp.x + nk_vp.w, nk_vp.y, WIN_WIDTH - nk_vp.w, WIN_HEIGHT);
#else
  SDL_Rect *game_vp = createRect(0, 0, WIN_WIDTH, WIN_HEIGHT);
#endif

  // Create the scene (map and player)
  Scene *scene = createScene();
  createTextures();

  // Main game loop

  EditorEventCtx event_ctx;
  event_ctx.quit = false;
  event_ctx.show_2d = false;
  event_ctx.game_focused = false;
  event_ctx.key_pressed = false;

  SDL_Event event;
  while (!event_ctx.quit)
  {
    handleWindowEvents(ctx, &event_ctx, window_main);

    SDL_RenderSetViewport(window_main->renderer, game_vp);
    renderScene(window_main->renderer, *scene, renderFpScene);

#ifdef DEBUG
    SDL_RenderSetViewport(window_main->renderer, NULL);
    render_nuklear(ctx, nk_vp, window_main->renderer);
#endif

    SDL_RenderPresent(window_main->renderer);
    updateFrameCounter(window_main);
    processPlayerMotion(&scene->player, 1 / window_main->fps, scene->map);
  }

// Cleanup and exit
#ifdef DEBUG
  nk_sdl_shutdown();
#endif

  freeScene(scene);
  freeWindowData(window_main);
  free(game_vp);
  IMG_Quit();
  SDL_Quit();
  return 0;
}
