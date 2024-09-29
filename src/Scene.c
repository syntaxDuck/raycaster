#include "Scene.h"

SDL_Renderer *renderer;

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

void draw2dScene(Scene scene, SDL_Renderer *rend)
{
  renderer = rend;
  draw2dMap(scene);
  draw2dPlayer(scene.player);
}

void draw2dMap(Scene scene)
{
  int col_offset;
  int row_offset;

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

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
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      }

      SDL_Rect rectangle = {col * (scene.map.unit_size), row * (scene.map.unit_size),
                            scene.map.unit_size - col_offset,
                            scene.map.unit_size - row_offset};
      SDL_RenderFillRect(renderer, &rectangle);
    }
  }
}

void draw2dPlayer(Player player)
{
  drawActor(player.actor);

  if (DEBUG)
  {
    drawPlayerViewRays(player);
    drawActorViewDir(player.actor);
    drawActorVelDir(player.actor);
    drawPlayerPlane(player);
  }
}

void drawPlayerPlane(Player player)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  Vector scaled_dir = player.actor.dir, scaled_plane = player.plane;
  scaleVector(&scaled_dir, 10);
  scaleVector(&scaled_plane, 5);
  SDL_RenderDrawLine(renderer, player.actor.pos.x + scaled_dir.x - scaled_plane.x,
                     player.actor.pos.y + scaled_dir.y - scaled_plane.y,
                     player.actor.pos.x + scaled_dir.x + scaled_plane.x,
                     player.actor.pos.y + scaled_dir.y + scaled_plane.y);
}

void drawActor(Actor actor)
{
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_Rect rect = {actor.pos.x - (actor.size >> 1),
                   actor.pos.y - (actor.size >> 1), actor.size, actor.size};
  SDL_RenderFillRect(renderer, &rect);
}

void drawActorViewDir(Actor actor)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  scaleVector(&actor.dir, 10);
  Vector view = transposeVector(actor.pos, actor.dir);
  SDL_RenderDrawLine(renderer, actor.pos.x, actor.pos.y, view.x,
                     view.y);
}

void drawActorVelDir(Actor actor)
{
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  Vector vel = actor.velocity;
  scaleVector(&vel, 10);
  vel = transposeVector(actor.pos, vel);
  SDL_RenderDrawLine(renderer, actor.pos.x, actor.pos.y, vel.x,
                     vel.y);
}

void drawActorViewRays(Actor actor)
{
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 75);
  for (int i = 0; i < NUM_RAYS; i++)
  {
    Vector ray = actor.view_cone[i];
    SDL_RenderDrawLine(renderer, actor.pos.x, actor.pos.y, ray.x,
                       ray.y);
  }
}

void drawPlayerViewRays(Player player)
{
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 75);
  for (int i = 0; i < WIN_WIDTH; i++)
  {
    Vector ray = player.intersects[i].vect;
    SDL_RenderDrawLine(renderer, player.actor.pos.x, player.actor.pos.y, ray.x * MAP_UNIT_SIZE,
                       ray.y * MAP_UNIT_SIZE);
  }
}

// void drawVectorFromActor(Actor actor, Vector vect, int scale, long color)
// {
//   SDL_SetRenderDrawColor(renderer_2d, color >> 24, color >> 16 & 0xFF, 255 >> 8 & 0xFF, color & 0xFF);
//   scaleVector(&vect, scale);
//   vect = transposeVector(actor.pos, vect);
//   SDL_RenderDrawLine(renderer_2d, actor.pos.x, actor.pos.y, vect.x, vect.y);
// }

void drawFpScene(Scene scene, SDL_Renderer *rend)
{
  renderer = rend;
  drawWalls(scene.player, scene);
}

void drawWalls(Player player, Scene scene)
{
  for (int x = 0; x < WIN_WIDTH; x++)
  {
    WallIntersect intersect = player.intersects[x];
    int line_height = (int)WIN_HEIGHT / intersect.perp_wall_distance;
    int draw_start = -line_height / 2 + WIN_HEIGHT / 2;
    if (draw_start < 0)
      draw_start = 0;
    int draw_end = line_height / 2 + WIN_HEIGHT / 2;
    if (draw_end >= WIN_HEIGHT)
      draw_end = WIN_HEIGHT - 1;

    // Select the texture based on the wall type (example: intersect.side could be used for this)
    int tex_num = scene.map.grid[(int)intersect.map_y][(int)intersect.map_x] - 1;
    // Calculate the exact x-coordinate on the texture
    double wall_x; // Exact position where the wall was hit
    if (intersect.side == 0)
      wall_x = intersect.vect.y;
    else
      wall_x = intersect.vect.x;
    wall_x -= floor(wall_x);

    // Calculate the x coordinate on the texture (from 0 to TEX_WIDTH - 1)
    int tex_x = (int)(wall_x * (double)TEX_WIDTH);
    if (intersect.side == 0 && intersect.ray_dir.x > 0)
      tex_x = TEX_WIDTH - tex_x - 1;
    if (intersect.side == 1 && intersect.ray_dir.y < 0)
      tex_x = TEX_WIDTH - tex_x - 1;

    // Draw the vertical wall slice
    for (int y = draw_start; y < draw_end; y++)
    {
      // Calculate the corresponding y position on the texture
      int tex_y = (((y * 2 - WIN_HEIGHT + line_height) * TEX_HEIGHT) / line_height) / 2;

      // Get the color from the texture
      Uint32 color = textures[tex_num][TEX_HEIGHT * tex_y + tex_x];

      // Modify color for shadows if hitting a horizontal wall
      if (intersect.side == 1)
        color = (color >> 1) & 0x7F7F7F; // Darken the color

      // Set the pixel color and draw the pixel
      SDL_SetRenderDrawColor(renderer,
                             (color & 0xFF0000) >> 16, // Red
                             (color & 0x00FF00) >> 8,  // Green
                             (color & 0x0000FF),       // Blue
                             255);                     // Alpha
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }
}

void processPlayerMotion(Scene *scene)
{
  Player *player = &scene->player;
  processActorMotion(&player->actor);
  rotateVector(&player->plane,
               player->actor.dir.angle - player->plane.angle + M_PI_2);
  castPlayerRays(player, *scene);
}

void freeScene(Scene *scene)
{
  // freePlayer(scene->player);
  freeMap(scene->map.grid, scene->map.height);
  free(scene);
}