#include <SDL2/SDL.h>

#include "Defines.h"
#include "Structs.h"
#include "Scene.h"

Player createPlayer()
{
  Player player;

  player.actor.size = PLAYER_SIZE;
  player.actor.field_of_view = PLAYER_FOV * DEG_TO_RAD;

  // Allocate memory for the player's view cone
  player.actor.view_cone = malloc(sizeof(Vector) * WIN_WIDTH);
  player.actor.max_vel = PLAYER_MAX_SPEED;
  player.actor.accel = PLAYER_ACCEL;

  // Initialize player's position and velocity
  player.actor.pos = setVector(2 * (double)WIN_WIDTH / 3 - 1, 2 * (double)WIN_HEIGHT / 3 - 1);
  player.actor.velocity = setVector(0, 0);
  player.actor.dir = setVector(-1, 0);

  // Calculate the player's plane (used for field of view in 3D rendering)
  player.plane = setVector(0, player.actor.dir.x * tan(player.actor.field_of_view / 2));
  player.intersects = malloc(sizeof(WallIntersect) * WIN_WIDTH);

  return player;
}

Scene *createScene()
{
  Scene *scene = malloc(sizeof(Scene));
  if (!scene)
  {
    fprintf(stderr, "Failed to allocate memory for scene\n");
    exit(EXIT_FAILURE);
  }

  // Initialize map
  Map map;
  map.unit_size = MAP_UNIT_SIZE;
  map.grid = loadMapFromFile("../assets/maps/map.txt", &map.width, &map.height);
  if (map.grid == NULL)
  {
    fprintf(stderr, "Failed to load map from file\n");
    exit(EXIT_FAILURE);
  }

  // Set the map and player for the scene
  scene->map = map;
  scene->player = createPlayer();
  return scene;
}

void createTextures()
{
  for (int x = 0; x < TEX_WIDTH; x++)
  {
    for (int y = 0; y < TEX_HEIGHT; y++)
    {
      int xorcolor = (x * 256 / TEX_WIDTH) ^ (y * 256 / TEX_HEIGHT);
      // int xcolor = x * 256 / TEX_WIDTH;
      int ycolor = y * 256 / TEX_HEIGHT;
      int xycolor = y * 128 / TEX_HEIGHT + x * 128 / TEX_WIDTH;
      textures[0][TEX_WIDTH * y + x] = 65536 * 254 * (x != y && x != TEX_WIDTH - y); // flat red texture with black cross
      textures[1][TEX_WIDTH * y + x] = xycolor + 256 * xycolor + 65536 * xycolor;    // sloped greyscale
      textures[2][TEX_WIDTH * y + x] = 256 * xycolor + 65536 * xycolor;              // sloped yellow gradient
      textures[3][TEX_WIDTH * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; // xor greyscale
      textures[4][TEX_WIDTH * y + x] = 256 * xorcolor;                               // xor green
      textures[5][TEX_WIDTH * y + x] = 65536 * 192 * (x % 16 && y % 16);             // red bricks
      textures[6][TEX_WIDTH * y + x] = 65536 * ycolor;                               // red gradient
      textures[7][TEX_WIDTH * y + x] = 128 + 256 * 128 + 65536 * 128;                // flat grey texture
    }
  }
}

int initSDL(WindowData *window_data, const char *title, int width, int height)
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return -1;
  }

  // Create the window
  window_data->window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
  if (window_data->window == NULL)
  {
    fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  // Create the renderer
  window_data->renderer = SDL_CreateRenderer(window_data->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (window_data->renderer == NULL)
  {
    fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window_data->window);
    SDL_Quit();
    return -1;
  }

  // Set the renderer color (optional)
  SDL_SetRenderDrawBlendMode(window_data->renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(window_data->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

  // Initialize frame count
  window_data->title = malloc(sizeof(char) * strlen(title));
  strcpy(window_data->title, title);

  window_data->frame_count = 0;
  window_data->fps = 0.0f;
  window_data->last_time = SDL_GetTicks();

  return 0;
}

void updateFrameCounter(WindowData *window_data)
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
    snprintf(title, sizeof(title), "%s - FPS: %.2f", window_data->title, window_data->fps);

    // Set the new window title with the FPS
    SDL_SetWindowTitle(window_data->window, title);
  }
}

bool handleScene(WindowData *window_data, Scene scene, void (*draw)(Scene, SDL_Renderer *))
{
  SDL_Event event;
  bool quit = false;
  while (SDL_PollEvent(&event) != 0)
  {
    if (event.type == SDL_QUIT)
    {
      quit = true;
    }
  }

  // Clear the screen
  SDL_SetRenderDrawColor(window_data->renderer, 0, 0, 0, 255);
  SDL_RenderClear(window_data->renderer);

  // Here you would draw your scene
  draw(scene, window_data->renderer);

  // Update the frame count in the window title
  updateFrameCounter(window_data);

  // Present the rendered frame to the screen
  SDL_RenderPresent(window_data->renderer);

  return quit;
}

void freeWindowData(WindowData *window_data)
{
  if (window_data->renderer)
    SDL_DestroyRenderer(window_data->renderer);
  if (window_data->window)
    SDL_DestroyWindow(window_data->window);

  free(window_data->title);
}

int main(int argc, char *argv[])
{
  WindowData window_2d, window_fp;
  // Initialize first-person rendering window and renderer
  if (REND_FP)
  {
    initSDL(&window_fp, "FP Viewport", WIN_WIDTH, WIN_HEIGHT);
  }

  // Initialize 2D rendering window and renderer
  if (REND_2D)
  {
    initSDL(&window_2d, "2D Viewport", WIN_WIDTH, WIN_HEIGHT);
  }

  // Create the scene (map and player)
  Scene *scene = createScene();
  createTextures();

  // Main game loop
  bool quit = false;
  while (!quit)
  {
    if (REND_2D)
    {
      quit = handleScene(&window_2d, *scene, render2dScene);
    }

    if (REND_FP)
    {
      quit = handleScene(&window_fp, *scene, drawFpScene);
    }
    Uint32 currentTime = SDL_GetTicks();
    Uint32 timeElapsed = currentTime - window_fp.last_time; // In milliseconds
    processPlayerMotion(scene, timeElapsed / 1000.0);
  }

  // Cleanup and exit
  freeScene(scene);
  freeWindowData(&window_2d);
  freeWindowData(&window_fp);
  SDL_Quit();
  return 0;
}
