#include "Scene.h"

SDL_Renderer *renderer;

Scene *createScene()
{
  Scene *scene = malloc(sizeof(Scene));
  if (!scene)
  {
    fprintf(stderr, "Failed to allocate memory for scene\n");
    exit(EXIT_FAILURE);
  }

  // Initialize map
  Map map = loadMap("../assets/maps/map.txt");
  if (map.walls == NULL || map.ceil == NULL || map.floor == NULL)
  {
    fprintf(stderr, "Failed to load map from file\n");
    exit(EXIT_FAILURE);
  }

  // Set the map and player for the scene
  scene->map = map;
  scene->player = createPlayer();
  return scene;
}

void render2dScene(Scene scene, SDL_Renderer *rend)
{
  renderer = rend;
  render2dMap(scene);
  render2dPlayer(scene.player);
}

void render2dMap(Scene scene)
{
  int x_offset;
  int y_offset;

  // Set the background color (white)
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  // Loop through the map and draw rectangles
  for (int y = 0; y < scene.map.height; y++)
  {
    y_offset = (y == scene.map.height - 1) ? 0 : 1;

    for (int x = 0; x < scene.map.width; x++)
    {
      x_offset = (x == scene.map.width - 1) ? 0 : 1;

      // Set the color depending on the grid value
      if (scene.map.walls[y][x])
      {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for filled cells
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black for empty cells
      }

      // Define the rectangle for each cell in the map
      SDL_Rect rectangle = {x * scene.map.unit_size, y * scene.map.unit_size,
                            scene.map.unit_size - x_offset,
                            scene.map.unit_size - y_offset};

      // Draw the rectangle onto the texture
      SDL_RenderFillRect(renderer, &rectangle);
    }
  }
}

void render2dPlayer(Player player)
{
  renderActorBody(player.actor);

  if (DEBUG)
  {
    renderPlayerViewRays(player);
    renderActorViewDir(player.actor);
    renderActorVelDir(player.actor);
    renderPlayerPlane(player);
  }
}

void renderPlayerPlane(Player player)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  setVectorMagnitude(&player.actor.dir, 10);
  setVectorMagnitude(&player.plane, 5);
  SDL_RenderDrawLine(renderer,
                     player.actor.pos.x + player.actor.dir.x - player.plane.x,
                     player.actor.pos.y + player.actor.dir.y - player.plane.y,
                     player.actor.pos.x + player.actor.dir.x + player.plane.x,
                     player.actor.pos.y + player.actor.dir.y + player.plane.y);
}

void renderActorBody(Actor actor)
{
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_Rect rect = {actor.pos.x - (actor.size >> 1),
                   actor.pos.y - (actor.size >> 1), actor.size, actor.size};
  SDL_RenderFillRect(renderer, &rect);
}

void renderActorViewDir(Actor actor)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  setVectorMagnitude(&actor.dir, 10);
  translateVector(&actor.dir, actor.pos);
  SDL_RenderDrawLine(renderer,
                     actor.pos.x,
                     actor.pos.y,
                     actor.dir.x,
                     actor.dir.y);
}

void renderActorVelDir(Actor actor)
{
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  setVectorMagnitude(&actor.velocity, 10);
  translateVector(&actor.pos, actor.velocity);
  SDL_RenderDrawLine(renderer,
                     actor.pos.x,
                     actor.pos.y,
                     actor.velocity.x,
                     actor.velocity.y);
}

void renderActorViewRays(Actor actor)
{
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 75);
  for (int i = 0; i < NUM_RAYS; i++)
  {
    Vector ray = actor.view_cone[i];
    SDL_RenderDrawLine(renderer,
                       actor.pos.x,
                       actor.pos.y,
                       ray.x,
                       ray.y);
  }
}

void renderPlayerViewRays(Player player)
{
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 75);
  for (int i = 0; i < WIN_WIDTH; i++)
  {
    Vector ray = player.intersects[i].vect;
    SDL_RenderDrawLine(renderer,
                       player.actor.pos.x,
                       player.actor.pos.y,
                       ray.x * MAP_UNIT_SIZE,
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

void renderFpScene(Scene scene, SDL_Renderer *rend)
{
  renderer = rend;
  renderFloorAndCeil(scene.player, scene.map);
  renderWalls(scene.player, scene.map);
}

void renderFloorAndCeil(Player player, Map map)
{
  SDL_Texture *texture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           WIN_WIDTH,
                                           WIN_HEIGHT);
  void *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, &pixels, &pitch);
  memset(pixels, 0xFFFFFF00, pitch * WIN_HEIGHT);
  Uint32 *pixel_data = (Uint32 *)pixels;
  Uint32 color;

  for (int y = WIN_HEIGHT / 2; y < WIN_HEIGHT; ++y)
  {
    // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
    float rayDirX0 = player.actor.dir.x - player.plane.x;
    float rayDirY0 = player.actor.dir.y - player.plane.y;
    float rayDirX1 = player.actor.dir.x + player.plane.x;
    float rayDirY1 = player.actor.dir.y + player.plane.y;

    // Current y position compared to the center of the screen (the horizon)
    int p = y - WIN_HEIGHT / 2;
    if (p == 0)
      p = 1; // Prevent division by zero
    if (p < 0)
      p = 1; // Avoid negative p values

    // Vertical position of the camera.
    // NOTE: with 0.5, it's exactly in the center between floor and ceiling,
    // matching also how the walls are being raycasted. For different values
    // than 0.5, a separate loop must be done for ceiling and floor since
    // they're no longer symmetrical.
    float posZ = 0.5 * WIN_HEIGHT;

    // Horizontal distance from the camera to the floor for the current row.
    // 0.5 is the z position exactly in the middle between floor and ceiling.
    // NOTE: this is affine textures mapping, which is not perspective correct
    // except for perfectly horizontal and vertical surfaces like the floor.
    // NOTE: this formula is explained as follows: The camera ray goes through
    // the following two points: the camera itself, which is at a certain
    // height (posZ), and a point in front of the camera (through an imagined
    // vertical plane containing the screen pixels) with horizontal distance
    // 1 from the camera, and vertical position p lower than posZ (posZ - p). When going
    // through that point, the line has vertically traveled by p units and
    // horizontally by 1 unit. To hit the floor, it instead needs to travel by
    // posZ units. It will travel the same ratio horizontally. The ratio was
    // 1 / p for going through the camera plane, so to go posZ times farther
    // to reach the floor, we get that the total horizontal distance is posZ / p.
    float rowDistance = posZ / p;

    // calculate the real world step vector we have to add for each x (parallel to camera plane)
    // adding step by step avoids multiplications with a weight in the inner loop
    float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / WIN_WIDTH;
    float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / WIN_WIDTH;

    // real world coordinates of the leftmost column. This will be updated as we step to the right.
    float floorX = player.actor.pos.x / MAP_UNIT_SIZE + rowDistance * rayDirX0;
    float floorY = player.actor.pos.y / MAP_UNIT_SIZE + rowDistance * rayDirY0;

    for (int x = 0; x < WIN_WIDTH; x++)
    {
      // the cell coord is simply got from the integer parts of floorX and floorY
      int cellX = (int)(floorX);
      int cellY = (int)(floorY);

      // if (x % 2 && y == WIN_HEIGHT / 2 + 1)
      //   printf("y: %d, x: %d, floorX: %f, floorY: %f, floorStepX: %f, floorStepY: %f, cellX: %d, cellY: %d\n", y, x, floorX, floorY, floorStepX, floorStepY, cellX, cellY);

      // get the textures coordinate from the fractional part
      int tx = (int)(TEX_WIDTH * (floorX - cellX)) % TEX_WIDTH;
      int ty = (int)(TEX_HEIGHT * (floorY - cellY)) % TEX_HEIGHT;

      // if (x == 0)
      //   printf("y: %d, x: %d, tx: %d, ty: %d\n", y, x, tx, ty);

      floorX += floorStepX;
      floorY += floorStepY;

      // floor
      if (cellX >= 0 && cellX <= 23 && cellY >= 0 && cellY <= 23)
      {
        // Floor
        color = textures[map.floor[cellY][cellX]][TEX_HEIGHT * ty + tx] << 8 | 0xFF;
        pixel_data[(y * (pitch / 4)) + x] = color;

        // Ceil
        cellY--;
        if (cellY < 0)
          cellY = 0;

        color = textures[map.ceil[cellY][cellX]][TEX_HEIGHT * ty + tx] << 8 | 0xFF;
        pixel_data[((WIN_HEIGHT - y) * (pitch / 4)) + x] = color;
      }
      else
      {
        pixel_data[(y * (pitch / 4)) + x] = 0xFF00FFFF;
        pixel_data[((WIN_HEIGHT - y) * (pitch / 4)) + x] = 0xFF00FFFF;
      }
    }
  }
  SDL_UnlockTexture(texture);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_DestroyTexture(texture);
}

void renderWalls(Player player, Map map)
{
  SDL_Texture *texture = SDL_CreateTexture(renderer,
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           WIN_WIDTH,
                                           WIN_HEIGHT);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  void *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, &pixels, &pitch);
  memset(pixels, 0, pitch * WIN_HEIGHT);
  Uint32 *pixel_data = (Uint32 *)pixels;
  Uint32 color;

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

    // Select the texture based on the walls type (example: intersect.side could be used for this)
    int tex_num = map.walls[(int)intersect.map_y][(int)intersect.map_x] - 1;
    // Calculate the exact x-coordinate on the texture
    double wall_x; // Exact position where the walls was hit
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

    // Draw the vertical walls slice
    for (int y = draw_start; y <= draw_end; y++)
    {
      // Calculate the corresponding y position on the texture
      int tex_y = (((y * 2 - WIN_HEIGHT + line_height) * TEX_HEIGHT) / line_height) / 2;

      // Get the color from the texture
      if (tex_num >= 0)
        color = textures[tex_num][TEX_HEIGHT * tex_y + tex_x] << 8 | 0xFF;
      else
        color = 0xFF00FFFF;

      // Modify color for shadows if hitting a horizontal walls
      // if (intersect.side == 1)
      //   color = ((color >> 9) & 0x7F7F7F) << 8 | 0xFF; // Darken the color

      // Set the pixel color and draw the pixel
      pixel_data[(y * (pitch / 4)) + x] = color;
    }
  }
  SDL_UnlockTexture(texture);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_DestroyTexture(texture);
}

void freeScene(Scene *scene)
{
  freePlayer(&scene->player);
  freeMap(scene->map);
  free(scene);
}
