#include "Actor.h"

void processActorMotion(Actor *actor, float frame_time)
{
  float movment_speed = 5 * MAP_UNIT_SIZE * frame_time;

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  // Rotation Function
  if (state[SDL_SCANCODE_D] ^ state[SDL_SCANCODE_A])
  {
    if (state[SDL_SCANCODE_A])
    {
      rotateVector(&actor->dir, -PLAYER_TURN_SPEED);
    }
    else
    {
      rotateVector(&actor->dir, PLAYER_TURN_SPEED);
    }
    actor->velocity.angle = actor->dir.angle;
  }

  // Velocity Vector Function
  if (state[SDL_SCANCODE_W] ^ state[SDL_SCANCODE_S])
  {
    if (state[SDL_SCANCODE_W])
    {
      actor->pos.x += actor->dir.x * movment_speed;
      actor->pos.y += actor->dir.y * movment_speed;
    }
    else
    {
      actor->pos.x -= actor->dir.x * movment_speed;
      actor->pos.y -= actor->dir.y * movment_speed;
    }
  }
}

Vector getRayRowIntersect(Vector origin, Vector ray, Scene scene)
{
  Vector casted_ray;
  double x, y;
  int row_index, col_index;

  double ray_cot = 1 / tan(ray.angle);
  int row_offset = origin.y - (int)origin.y % MAP_UNIT_SIZE;
  while (true)
  {
    if (ray.angle == 0 || ray.angle == M_PI)
    {
      x = ray.angle == 0 ? 1e30 : -1e30;
      y = 0;

      casted_ray.x = origin.x - x;
      casted_ray.y = origin.y + y;
      casted_ray.mag = sqrt(x * x + y * y);
      casted_ray.angle = ray.angle;
      break;
    }

    else
    {
      if (ray.angle > 0 && ray.angle < M_PI)
      {
        x = (origin.y - row_offset) * ray_cot;
        y = row_offset - origin.y;

        row_offset -= MAP_UNIT_SIZE;
        row_index = -1;
      }
      else
      {
        row_offset += MAP_UNIT_SIZE;
        x = -(row_offset - origin.y) * ray_cot;
        y = row_offset - origin.y;
        row_index = 0;
      }

      casted_ray.x = origin.x - x;
      casted_ray.y = origin.y + y;
      casted_ray.mag = sqrt(x * x + y * y);
      casted_ray.angle = ray.angle;

      row_index += casted_ray.y / MAP_UNIT_SIZE;
      col_index = casted_ray.x / MAP_UNIT_SIZE;

      if (row_index < 0)
        row_index = 0;

      if (row_index >= scene.map.height)
        row_index = scene.map.height - 1;

      if (casted_ray.x < 0)
        break;

      if (casted_ray.x > scene.map.width * MAP_UNIT_SIZE)
        break;

      if (scene.map.grid[row_index][col_index] > 0)
      {
        break;
      }
    }
  }
  return casted_ray;
}

Vector getRayColIntersect(Vector origin, Vector ray, Scene scene)
{
  Vector casted_ray;
  double x, y;
  int row_index, col_index;

  double ray_tan = tan(ray.angle);
  int col_offset = origin.x - (int)origin.x % MAP_UNIT_SIZE;
  while (true)
  {
    if (ray.angle == M_PI_2 ||
        ray.angle == M_PI_3_2)
    {

      y = ray.angle == M_PI_2 ? 1e30 : -1e30;
      x = 0;

      casted_ray.x = origin.x + x;
      casted_ray.y = origin.y - y;
      casted_ray.mag = sqrt(x * x + y * y);
      casted_ray.angle = ray.angle;
      break;
    }

    else
    {

      if (ray.angle < M_PI_2 ||
          ray.angle > M_PI_3_2)
      {
        x = col_offset - origin.x;
        y = (origin.x - col_offset) * ray_tan;

        col_offset -= MAP_UNIT_SIZE;
        col_index = -1;
      }
      else
      {
        col_offset += MAP_UNIT_SIZE;
        x = col_offset - origin.x;
        y = -(col_offset - origin.x) * ray_tan;
        col_index = 0;
      }

      casted_ray.x = origin.x + x;
      casted_ray.y = origin.y - y;
      casted_ray.mag = sqrt(x * x + y * y);
      casted_ray.angle = ray.angle;

      row_index = casted_ray.y / MAP_UNIT_SIZE;
      col_index += casted_ray.x / MAP_UNIT_SIZE;

      if (col_index < 0)
        col_index = 0;

      if (col_index >= scene.map.width)
        col_index = scene.map.width - 1;

      if (casted_ray.y < 0)
        break;

      if (casted_ray.y > scene.map.height * MAP_UNIT_SIZE)
        break;

      if (scene.map.grid[row_index][col_index] > 0)
      {
        break;
      }
    }
  }
  return casted_ray;
}

void castActorRays(Actor *actor, Scene scene)
{
  double increment_rad = actor->field_of_view / NUM_RAYS;
  double starting_angle = -actor->field_of_view / 2 + increment_rad;
  for (int i = 0; i < NUM_RAYS; i++)
  {
    Vector new_vect = actor->dir;
    rotateVector(&new_vect,
                 starting_angle + i * increment_rad);

    Vector row_intersect = getRayRowIntersect(actor->pos, new_vect, scene);
    Vector col_intersect = getRayColIntersect(actor->pos, new_vect, scene);
    actor->view_cone[i] = row_intersect.mag < col_intersect.mag ? row_intersect : col_intersect;
  }
}

WallIntersect getIntersect(Vector origin, Vector ray_dir, Scene scene)
{
  Vector map = setVector((int)origin.x, (int)origin.y);
  Vector side_dist;
  Vector delta_dist = setVector(ray_dir.x == 0 ? 1e30 : fabs(1 / ray_dir.x),
                                ray_dir.y == 0 ? 1e30 : fabs(1 / ray_dir.y));
  double perp_wall_dist;
  Vector step;
  int hit = 0;
  int side;
  if (ray_dir.x < 0)
  {
    step.x = -1;
    side_dist.x = (origin.x - map.x) * delta_dist.x;
  }
  else
  {
    step.x = 1;
    side_dist.x = (map.x + 1.0 - origin.x) * delta_dist.x;
  }

  if (ray_dir.y < 0)
  {
    step.y = -1;
    side_dist.y = (origin.y - map.y) * delta_dist.y;
  }
  else
  {
    step.y = 1;
    side_dist.y = (map.y + 1.0 - origin.y) * delta_dist.y;
  }

  while (hit == 0)
  {
    if (side_dist.x < side_dist.y)
    {
      side_dist.x += delta_dist.x;
      map.x += step.x;
      side = 0;
    }
    else
    {
      side_dist.y += delta_dist.y;
      map.y += step.y;
      side = 1;
    }

    if (scene.map.grid[(int)map.y][(int)map.x] > 0)
      hit = 1;
  }
  if (side == 0)
    perp_wall_dist = side_dist.x - delta_dist.x;
  else
    perp_wall_dist = side_dist.y - delta_dist.y;

  WallIntersect intersect;
  intersect.vect = setVector(origin.x + ray_dir.x * perp_wall_dist, origin.y + ray_dir.y * perp_wall_dist);
  intersect.perp_wall_distance = perp_wall_dist;
  intersect.side = side;
  intersect.map_x = map.x;
  intersect.map_y = map.y;
  intersect.ray_dir = ray_dir;
  return intersect;
}

void castPlayerRays(Player *player, Scene scene)
{
  Vector dir = player->actor.dir; // Player's direction vector
  Vector plane = player->plane;   // Player's plane vector (perpendicular to direction)
  Vector pos = setVector(player->actor.pos.x / MAP_UNIT_SIZE, player->actor.pos.y / MAP_UNIT_SIZE);

  double rad_per_col = player->actor.field_of_view / WIN_WIDTH; // Radians per pixel
  double offset = -player->actor.field_of_view / 2;
  double camera_x;

  for (int x = 0; x < WIN_WIDTH; x++)
  {
    // Calculate x-coordinate in camera space
    camera_x = 2 * x / (double)WIN_WIDTH - 1; // Normalized coordinate in camera space
    Vector ray_dir = setVector(dir.x + plane.x * camera_x, dir.y + plane.y * camera_x);
    player->intersects[x] = getIntersect(pos, ray_dir, scene);
    player->actor.view_cone[x] = player->intersects[x].vect;
  }
}

void freePlayer(Player *player)
{
  free(player->actor.view_cone);
  free(player->intersects);
}