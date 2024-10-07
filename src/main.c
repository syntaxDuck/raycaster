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

void render_nuklear(struct nk_context *ctx, SDL_Renderer *renderer)
{
  if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 230),
               NK_WINDOW_BORDER | NK_WINDOW_MOVABLE |
                   NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE |
                   NK_WINDOW_TITLE))
  {
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

// TODO: this function seems to be throwing an exception when multipl keys are pressed
void handleEvents(struct nk_context *ctx, SDL_Event *event, bool *quit,
                  bool *key_pressed, bool *show_2d,
                  WindowData *window_main, WindowData *window_2d)
{
  // Handle events
  nk_input_begin(ctx);
  while (SDL_PollEvent(event) != 0)
  {
    if (event->type == SDL_QUIT)
    {
      *quit = true;
    }
    if (event->type == SDL_KEYDOWN && !*key_pressed)
    {
      *key_pressed = true;
      const Uint8 *state = SDL_GetKeyboardState(NULL);
      if (state[SDL_SCANCODE_2])
      {
        if (!*show_2d)
        {
          int x, y;
          SDL_GetWindowPosition(window_main->window, &x, &y);
          window_2d = createWindow("2D Viewport",
                                   x - WIN_WIDTH,
                                   y, WIN_WIDTH,
                                   WIN_HEIGHT);
          *show_2d = true;
        }
        else
        {
          freeWindowData(window_2d);
          SDL_RaiseWindow(window_main->window);
          window_2d = NULL;
          show_2d = false;
        }
      }
    }
    if (event->type == SDL_KEYUP)
    {
      key_pressed = false;
    }
    nk_sdl_handle_event(event);
  }
  nk_input_end(ctx);
}
struct nk_context *setupMenu(WindowData *window)
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

  WindowData *window_2d;
  WindowData *window_main = createWindow("Main Viewport",
                                         SDL_WINDOWPOS_CENTERED,
                                         SDL_WINDOWPOS_CENTERED,
                                         WIN_WIDTH, WIN_HEIGHT);

  struct nk_context *ctx = setupMenu(window_main);

  // Create the scene (map and player)
  Scene *scene = createScene();
  createTextures();

  // // Main game loop
  SDL_Event event;
  bool quit = false;
  bool show_2d = false;
  bool key_pressed = false;
  while (!quit)
  {
    handleEvents(ctx, &event, &quit, &key_pressed,
                 &show_2d, window_main, window_2d);

    renderScene(window_main->renderer, *scene, renderFpScene);
    render_nuklear(ctx, window_main->renderer);
    SDL_RenderPresent(window_main->renderer);
    updateFrameCounter(window_main);

    if (show_2d)
    {
      renderScene(window_2d->renderer, *scene, render2dScene);
    }

    processPlayerMotion(&scene->player, 1 / window_main->fps, scene->map);
  }

  // Cleanup and exit
  nk_sdl_shutdown();
  freeScene(scene);
  freeWindowData(window_main);
  freeWindowData(window_2d);
  IMG_Quit();
  SDL_Quit();
  return 0;
}
