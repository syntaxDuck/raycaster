#include "Scene.h"

Scene *createScene(char *map_path)
{
  Scene *scene = malloc(sizeof(Scene));
  if (!scene)
  {
    fprintf(stderr, "Failed to allocate memory for scene\n");
    exit(1);
  }

  // Initialize map
  Map map = loadMap(map_path);
  if (map.walls == NULL || map.ceil == NULL || map.floor == NULL)
  {
    fprintf(stderr, "Failed to load map from file\n");
    exit(1);
  }

  Texture *textures = createTextures();
  if (textures == NULL)
  {
    fprintf(stderr, "Failed to load textures\n");
    exit(1);
  }

  SceneStaticSprites s_sprites;
  s_sprites.sprites = malloc(sizeof(StaticSprite) * 10);
  if (s_sprites.sprites == NULL)
  {
    fprintf(stderr, "Failed to allocate memory for static sprits\n");
    exit(1);
  }
  s_sprites.sprites[0].pos = setVector(10.5, 10.5);
  s_sprites.sprites[0].texture = 10;
  s_sprites.sprites[1].pos = setVector(15.5, 15.5);
  s_sprites.sprites[1].texture = 9;
  s_sprites.sprites[2].pos = setVector(5.5, 5.5);
  s_sprites.sprites[2].texture = 8;
  s_sprites.num_sprites = 3;
  s_sprites.sprite_order = malloc(sizeof(int) * s_sprites.num_sprites);
  s_sprites.sprite_dist = malloc(sizeof(double) * s_sprites.num_sprites);

  // Set the map and player for the scene
  scene->map = map;
  scene->player = createPlayer();
  scene->s_sprites = s_sprites;
  scene->textures = textures;

  return scene;
}

void renderScene(
    Scene scene,
    void (*render)(Scene))
{

  // Clear the screen
  SDL_SetRenderDrawColor(win_ctx->renderer, 0, 0, 0, 255);
  SDL_RenderClear(win_ctx->renderer);

  // Here you would draw your scene
  render(scene);

  // Present the rendered frame to the screen
}

void render2dScene(Scene scene)
{
  render2dMap(scene);
  render2dPlayer(scene.player);
}

void render2dMap(Scene scene)
{
  int x_offset;
  int y_offset;

  // Set the background color (white)
  SDL_SetRenderDrawColor(win_ctx->renderer, 255, 255, 255, 255);
  SDL_RenderClear(win_ctx->renderer);

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
        SDL_SetRenderDrawColor(win_ctx->renderer, 255, 0, 0, 255); // Red for filled cells
      }
      else
      {
        SDL_SetRenderDrawColor(win_ctx->renderer, 0, 0, 0, 255); // Black for empty cells
      }

      // Define the rectangle for each cell in the map
      SDL_Rect rectangle = {x * scene.map.unit_size, y * scene.map.unit_size,
                            scene.map.unit_size - x_offset,
                            scene.map.unit_size - y_offset};

      // Draw the rectangle onto the texture
      SDL_RenderFillRect(win_ctx->renderer, &rectangle);
    }
  }
}

void render2dPlayer(Player player)
{
  renderActorBody(player.actor);

#ifdef DEBUG
  renderPlayerViewRays(player);
  renderActorViewDir(player.actor);
  renderActorVelDir(player.actor);
  renderPlayerPlane(player);
#endif
}

void renderPlayerPlane(Player player)
{
  SDL_SetRenderDrawColor(win_ctx->renderer, 0, 0, 0, 255);
  setVectorMagnitude(&player.actor.dir, 10);
  setVectorMagnitude(&player.plane, 5);
  SDL_RenderDrawLine(win_ctx->renderer,
                     player.actor.pos.x + player.actor.dir.x - player.plane.x,
                     player.actor.pos.y + player.actor.dir.y - player.plane.y,
                     player.actor.pos.x + player.actor.dir.x + player.plane.x,
                     player.actor.pos.y + player.actor.dir.y + player.plane.y);
}

void renderActorBody(Actor actor)
{
  SDL_SetRenderDrawColor(win_ctx->renderer, 0, 255, 0, 255);
  SDL_Rect rect = {actor.pos.x - (actor.size >> 1),
                   actor.pos.y - (actor.size >> 1), actor.size, actor.size};
  SDL_RenderFillRect(win_ctx->renderer, &rect);
}

void renderActorViewDir(Actor actor)
{
  SDL_SetRenderDrawColor(win_ctx->renderer, 0, 0, 0, 255);
  setVectorMagnitude(&actor.dir, 10);
  translateVector(&actor.dir, actor.pos);
  SDL_RenderDrawLine(win_ctx->renderer,
                     actor.pos.x,
                     actor.pos.y,
                     actor.dir.x,
                     actor.dir.y);
}

void renderActorVelDir(Actor actor)
{
  SDL_SetRenderDrawColor(win_ctx->renderer, 255, 255, 255, 255);
  setVectorMagnitude(&actor.velocity, 10);
  translateVector(&actor.pos, actor.velocity);
  SDL_RenderDrawLine(win_ctx->renderer,
                     actor.pos.x,
                     actor.pos.y,
                     actor.velocity.x,
                     actor.velocity.y);
}

void renderActorViewRays(Actor actor)
{
  SDL_SetRenderDrawColor(win_ctx->renderer, 255, 0, 255, 75);
  for (int i = 0; i < NUM_RAYS; i++)
  {
    Vector ray = actor.view_cone[i];
    SDL_RenderDrawLine(win_ctx->renderer,
                       actor.pos.x,
                       actor.pos.y,
                       ray.x,
                       ray.y);
  }
}

void renderPlayerViewRays(Player player)
{
  SDL_SetRenderDrawColor(win_ctx->renderer, 255, 0, 255, 75);
  for (int i = 0; i < win_ctx->width; i++)
  {
    Vector ray = player.intersects[i].vect;
    SDL_RenderDrawLine(win_ctx->renderer,
                       player.actor.pos.x,
                       player.actor.pos.y,
                       ray.x * MAP_UNIT_SIZE,
                       ray.y * MAP_UNIT_SIZE);
  }
}

void renderFpScene(Scene scene)
{
  renderFloorAndCeil(scene);
  renderWalls(scene);
  rendererSprites(scene);
}

void rendererSprites(Scene scene)
{
  Vector player_pos = scene.player.actor.pos;
  player_pos.x /= MAP_UNIT_SIZE;
  player_pos.y /= MAP_UNIT_SIZE;

  SDL_Texture *texture = SDL_CreateTexture(win_ctx->renderer,
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           win_ctx->width,
                                           win_ctx->height);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  void *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, &pixels, &pitch);
  memset(pixels, 0xFFFFFF00, pitch * win_ctx->height);
  Uint32 *pixel_data = (Uint32 *)pixels;
  Uint32 color;

  SceneStaticSprites s_sprites = scene.s_sprites;

  for (int i = 0; i < s_sprites.num_sprites; i++)
  {
    s_sprites.sprite_order[i] = i;
    s_sprites.sprite_dist[i] = ((player_pos.x - s_sprites.sprites[i].pos.x) *
                                    (player_pos.x - s_sprites.sprites[i].pos.x) +
                                (player_pos.y - s_sprites.sprites[i].pos.y) *
                                    (player_pos.y - s_sprites.sprites[i].pos.y));
  }
  sortSprites(s_sprites.sprite_order, s_sprites.sprite_dist, s_sprites.num_sprites);

  for (int i = 0; i < s_sprites.num_sprites; i++)
  {
    Vector rel_sprite_pos = setVector(s_sprites.sprites[s_sprites.sprite_order[i]].pos.x - player_pos.x,
                                      s_sprites.sprites[s_sprites.sprite_order[i]].pos.y - player_pos.y);

    double inv_det = 1.0 / (scene.player.plane.x * scene.player.actor.dir.y - scene.player.actor.dir.x * scene.player.plane.y);
    Vector transform = setVector(inv_det * (scene.player.actor.dir.y * rel_sprite_pos.x - scene.player.actor.dir.x * rel_sprite_pos.y),
                                 inv_det * (-scene.player.plane.y * rel_sprite_pos.x + scene.player.plane.x * rel_sprite_pos.y));
    int sprite_screen_x = (int)((win_ctx->width / 2) * (1 + transform.x / transform.y));
    int sprite_height = abs((int)(win_ctx->height / transform.y));

    int draw_start_y = -sprite_height / 2 + win_ctx->height / 2;
    if (draw_start_y < 0)
      draw_start_y = 0;

    int draw_end_y = sprite_height / 2 + win_ctx->height / 2;
    if (draw_end_y >= win_ctx->height)
      draw_end_y = win_ctx->height - 1;

    int sprite_width = abs((int)(win_ctx->height / transform.y));
    int draw_start_x = -sprite_width / 2 + sprite_screen_x;
    if (draw_start_x < 0)
      draw_start_x = 0;

    int draw_end_x = sprite_width / 2 + sprite_screen_x;
    if (draw_end_x >= win_ctx->width)
      draw_end_x = win_ctx->width - 1;

    for (int stripe = draw_start_x; stripe < draw_end_x; stripe++)
    {
      int tex_x = (int)(256 * (stripe - (-sprite_width / 2 + sprite_screen_x)) * TEX_WIDTH / sprite_width) / 256;
      if (transform.y > 0 && stripe > 0 && stripe < win_ctx->width && transform.y < scene.player.intersects[stripe].perp_wall_distance)
      {
        for (int y = draw_start_y; y < draw_end_y; y++)
        {
          int d = (y) * 256 - win_ctx->height * 128 + sprite_height * 128;
          int tex_y = ((d * TEX_HEIGHT) / sprite_height) / 256;
          color = scene.textures[s_sprites.sprites[s_sprites.sprite_order[i]].texture].pixels[TEX_WIDTH * tex_y + tex_x];

          if ((color & 0xFF) == 0xFF)
            pixel_data[(y * (pitch / 4)) + stripe] = color;
        }
      }
    }
  }
  SDL_UnlockTexture(texture);
  SDL_RenderCopy(win_ctx->renderer, texture, NULL, NULL);
  SDL_DestroyTexture(texture);
}

void renderFloorAndCeil(Scene scene)
{
  SDL_Texture *texture = SDL_CreateTexture(win_ctx->renderer,
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           win_ctx->width,
                                           win_ctx->height);
  void *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, &pixels, &pitch);
  memset(pixels, 0xFFFFFF00, pitch * win_ctx->height);
  Uint32 *pixel_data = (Uint32 *)pixels;
  Uint32 color;

  int w, h;
  SDL_GetWindowSizeInPixels(win_ctx->window, &w, &h);
  for (int y = h / 2; y < h; ++y)
  {
    // rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
    float rayDirX0 = scene.player.actor.dir.x - scene.player.plane.x;
    float rayDirY0 = scene.player.actor.dir.y - scene.player.plane.y;
    float rayDirX1 = scene.player.actor.dir.x + scene.player.plane.x;
    float rayDirY1 = scene.player.actor.dir.y + scene.player.plane.y;

    // Current y position compared to the center of the screen (the horizon)
    int p = y - h / 2;
    if (p == 0)
      p = 1; // Prevent division by zero
    if (p < 0)
      p = 1; // Avoid negative p values

    // Vertical position of the camera.
    // NOTE: with 0.5, it's exactly in the center between floor and ceiling,
    // matching also how the walls are being raycasted. For different values
    // than 0.5, a separate loop must be done for ceiling and floor since
    // they're no longer symmetrical.
    float posZ = 0.5 * h;

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
    float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / w;
    float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / w;

    // real world coordinates of the leftmost column. This will be updated as we step to the right.
    float floorX = scene.player.actor.pos.x / MAP_UNIT_SIZE + rowDistance * rayDirX0;
    float floorY = scene.player.actor.pos.y / MAP_UNIT_SIZE + rowDistance * rayDirY0;

    for (int x = 0; x < w; x++)
    {
      // the cell coord is simply got from the integer parts of floorX and floorY
      int cellX = (int)(floorX);
      int cellY = (int)(floorY);

      // if (x % 2 && y == game_window.height / 2 + 1)
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
        int texture_index = scene.map.floor[cellY][cellX];
        color = scene.textures[scene.map.floor[cellY][cellX]]
                    .pixels[TEX_HEIGHT * ty + tx];
        pixel_data[(y * (pitch / 4)) + x] = color;

        // Ceil
        cellY--;
        if (cellY < 0)
          cellY = 0;

        color = scene.textures[scene.map.ceil[cellY][cellX]].pixels[TEX_HEIGHT * ty + tx];
        pixel_data[((h - y) * (pitch / 4)) + x] = color;
      }
      else
      {
        pixel_data[(y * (pitch / 4)) + x] = 0xFF00FFFF;
        pixel_data[((h - y) * (pitch / 4)) + x] = 0xFF00FFFF;
      }
    }
  }
  SDL_UnlockTexture(texture);
  SDL_RenderCopy(win_ctx->renderer, texture, NULL, NULL);
  SDL_DestroyTexture(texture);
}

void renderWalls(Scene scene)
{
  SDL_Texture *texture = SDL_CreateTexture(win_ctx->renderer,
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           win_ctx->width,
                                           win_ctx->height);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
  void *pixels;
  int pitch;
  SDL_LockTexture(texture, NULL, &pixels, &pitch);
  memset(pixels, 0, pitch * win_ctx->height);
  Uint32 *pixel_data = (Uint32 *)pixels;
  Uint32 color;

  int w, h;
  SDL_GetWindowSizeInPixels(win_ctx->window, &w, &h);
  for (int x = 0; x < w; x++)
  {
    WallIntersect intersect = scene.player.intersects[x];
    int line_height = (int)h / intersect.perp_wall_distance;
    int draw_start = -line_height / 2 + h / 2;
    if (draw_start < 0)
      draw_start = 0;
    int draw_end = line_height / 2 + h / 2;
    if (draw_end >= h)
      draw_end = h - 1;

    // TODO: This is causing segfaults, currently patched with if statements
    // Select the texture based on the walls type (example: intersect.side could be used for this)
    if (intersect.map_x < 0 || intersect.map_x > scene.map.width)
      intersect.map_x = 0;
    if (intersect.map_y < 0 || intersect.map_y > scene.map.height)
      intersect.map_y = 0;
    int tex_num =
        scene.map.walls[intersect.map_y][intersect.map_x] - 1;
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
      int tex_y = (((y * 2 - win_ctx->height + line_height) * TEX_HEIGHT) / line_height) / 2;

      // Get the color from the texture
      if (tex_num >= 0)
        color = scene.textures[tex_num].pixels[TEX_HEIGHT * tex_y + tex_x];
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
  SDL_RenderCopy(win_ctx->renderer, texture, NULL, NULL);
  SDL_DestroyTexture(texture);
}

void freeScene(Scene *scene)
{
  freePlayer(&scene->player);
  freeMap(scene->map);
  free(scene);
}
