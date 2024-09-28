#include "Scene.h"

SDL_Renderer *renderer_2d, *renderer_fp;

void set2dSceneRenderer(SDL_Renderer *rend)
{
  renderer_2d = rend;
}

void setFpSceneRenderer(SDL_Renderer *rend)
{
  renderer_fp = rend;
}

#include <stdio.h>
#include <stdlib.h>

Uint8 **loadMapFromFile(const char *filename, int *map_width, int *map_height)
{
  // Open the file
  FILE *file = fopen(filename, "r");
  if (!file)
  {
    fprintf(stderr, "Failed to open map file: %s\n", filename);
    return NULL;
  }

  // Read the width and height of the map
  fscanf(file, "%d %d", map_width, map_height);

  // Allocate memory for the map
  Uint8 **map = malloc(*map_height * sizeof(Uint8 *));
  if (!map)
  {
    fprintf(stderr, "Failed to allocate memory for map\n");
    fclose(file);
    return NULL;
  }

  for (int i = 0; i < *map_height; i++)
  {
    map[i] = malloc(*map_width * sizeof(Uint8));
    if (!map[i])
    {
      fprintf(stderr, "Failed to allocate memory for map row\n");
      fclose(file);
      return NULL;
    }

    // Read each value into the map
    for (int j = 0; j < *map_width; j++)
    {
      if (fscanf(file, "%hhu", &map[i][j]) != 1)
      {
        fprintf(stderr, "Failed to read value for map[%d][%d]\n", i, j);
        fclose(file);
        return NULL;
      }
    }
  }

  fclose(file);
  return map;
}

// Function to print the map values to the console
void printMap(Uint8 **map, int map_width, int map_height)
{
  for (int i = 0; i < map_height; i++)
  {
    for (int j = 0; j < map_width; j++)
    {
      printf("%d ", map[i][j]); // Print each value in the row
    }
    printf("\n"); // Newline after each row
  }
}

// Function to free the dynamically allocated map memory
void freeMap(Uint8 **map, int map_height)
{
  for (int i = 0; i < map_height; i++)
  {
    free(map[i]);
  }
  free(map);
}

// void loadMap(Map *map, Uint8 grid[MAP_HEIGHT][MAP_WIDTH])
// {
//   grid = malloc(map->height * sizeof(Uint8 *));
//   for (int i = 0; i < map->height; i++)
//   {
//     map->grid[i] = malloc(map->width * sizeof(Uint8));
//   }

//   // Copy map data into map->map
//   for (int row = 0; row < map->height; row++)
//   {
//     for (int col = 0; col < map->width; col++)
//     {
//       map->grid[row][col] = grid[row][col];
//     }
//   }
// }

void draw2DScene(Scene scene)
{
  draw2dMap(scene);
  draw2dPlayer(scene.player);
  SDL_RenderPresent(renderer_2d);
}

void draw2dMap(Scene scene)
{
  int col_offset;
  int row_offset;

  SDL_SetRenderDrawColor(renderer_2d, 255, 255, 255, 255);
  SDL_RenderClear(renderer_2d);

  for (int row = 0; row < scene.map.height; row++)
  {
    if (row == scene.map.height - 1)
      row_offset = 0;
    else
      row_offset = 1;

    for (int col = 0; col < scene.map.width; col++)
    {

      if (col == scene.map.width - 1)
        col_offset = 0;
      else
        col_offset = 1;

      if (scene.map.grid[row][col])
      {
        SDL_SetRenderDrawColor(renderer_2d, 255, 0, 0, 255);
      }
      else
      {
        SDL_SetRenderDrawColor(renderer_2d, 0, 0, 0, 255);
      }

      SDL_Rect rectangle = {col * (scene.map.unit_size), row * (scene.map.unit_size),
                            scene.map.unit_size - col_offset,
                            scene.map.unit_size - row_offset};
      SDL_RenderFillRect(renderer_2d, &rectangle);
    }
  }
}

void draw2dPlayer(Player player)
{
  drawActor(player.actor);

  if (DEBUG)
  {
    drawActorViewDir(player.actor);
    drawActorVelDir(player.actor);
    drawPlayerViewRays(player);
    drawPlayerPlane(player);
  }
}

void drawPlayerPlane(Player player)
{
  SDL_SetRenderDrawColor(renderer_2d, 255, 0, 0, 255);
  SDL_RenderDrawLine(renderer_2d, player.actor.pos.x + player.actor.dir.x - player.plane.x,
                     player.actor.pos.y + player.actor.dir.y - player.plane.y,
                     player.actor.pos.x + player.actor.dir.x + player.plane.x,
                     player.actor.pos.y + player.actor.dir.y + player.plane.y);
}

void drawActor(Actor actor)
{
  SDL_SetRenderDrawColor(renderer_2d, 0, 255, 0, 255);
  SDL_Rect rect = {actor.pos.x - (actor.size >> 1),
                   actor.pos.y - (actor.size >> 1), actor.size, actor.size};
  SDL_RenderFillRect(renderer_2d, &rect);
}

void drawActorViewDir(Actor actor)
{
  SDL_SetRenderDrawColor(renderer_2d, 255, 0, 0, 255);
  Vector view = transposeVector(actor.pos, actor.dir);
  SDL_RenderDrawLine(renderer_2d, actor.pos.x, actor.pos.y, view.x,
                     view.y);
}

void drawActorVelDir(Actor actor)
{
  SDL_SetRenderDrawColor(renderer_2d, 0, 0, 255, 255);
  Vector vel = actor.velocity;
  scaleVector(&vel, 10);
  vel = transposeVector(actor.pos, vel);
  SDL_RenderDrawLine(renderer_2d, actor.pos.x, actor.pos.y, vel.x,
                     vel.y);
}

void drawActorViewRays(Actor actor)
{
  SDL_SetRenderDrawColor(renderer_2d, 255, 0, 255, 75);
  for (int i = 0; i < NUM_RAYS; i++)
  {
    Vector ray = actor.view_cone[i];
    SDL_RenderDrawLine(renderer_2d, actor.pos.x, actor.pos.y, ray.x,
                       ray.y);
  }
}

void drawPlayerViewRays(Player player)
{
  SDL_SetRenderDrawColor(renderer_2d, 255, 0, 255, 75);
  for (int i = 0; i < WIN_WIDTH; i++)
  {
    Vector ray = player.actor.view_cone[i];
    SDL_RenderDrawLine(renderer_2d, player.actor.pos.x, player.actor.pos.y, ray.x,
                       ray.y);
  }
}

// void drawVectorFromActor(Actor actor, Vector vect, int scale, long color)
// {
//   SDL_SetRenderDrawColor(renderer_2d, color >> 24, color >> 16 & 0xFF, 255 >> 8 & 0xFF, color & 0xFF);
//   scaleVector(&vect, scale);
//   vect = transposeVector(actor.pos, vect);
//   SDL_RenderDrawLine(renderer_2d, actor.pos.x, actor.pos.y, vect.x, vect.y);
// }

void drawFpScene(Scene *scene)
{
  return;
}

// void process2DScene(Scene *scene)
// {
//   processPlayerMotion(scene);
// }

void processPlayerMotion(Scene *scene)
{
  Player *player = &scene->player;
  processActorMotion(&player->actor);
  rotateVector(&player->plane,
               player->actor.dir.angle - player->plane.angle + M_PI_2);
  castPlayerRays(player, *scene);
  printf("%f\n", player->actor.dir.angle);
}