#include "Actor.h"

void processActorMotion(Actor *actor)
{

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  // Rotation Function
  if (state[SDL_SCANCODE_RIGHT] ^ state[SDL_SCANCODE_LEFT])
  {
    if (state[SDL_SCANCODE_LEFT])
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
  if (state[SDL_SCANCODE_UP] ^ state[SDL_SCANCODE_DOWN])
  {
    if (state[SDL_SCANCODE_UP])
    {
      actor->velocity.angle = actor->dir.angle;
    }
    else
    {
      actor->velocity.angle = actor->dir.angle + M_PI;
    }

    if ((actor->velocity.mag + actor->accel) >
        actor->max_vel)
    {
      rescaleVector(&actor->velocity, actor->max_vel);
    }
    else
    {
      scaleVector(&actor->velocity, actor->accel);
    }
  }

  else
  {
    rescaleVector(&actor->velocity, 0);
  }

  actor->pos =
      transposeVector(actor->pos, actor->velocity);
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

void castPlayerRays(Player *player, Scene scene)
{
  Vector dir = player->actor.dir; // Player's direction vector
  Vector plane = player->plane;   // Player's plane vector (perpendicular to direction)

  double rad_per_col = player->actor.field_of_view / WIN_WIDTH; // Radians per pixel
  double offset = -player->actor.field_of_view / 2;
  double camera_x;

  for (int x = 0; x < WIN_WIDTH; x++)
  {
    // Calculate x-coordinate in camera space
    camera_x = 2 * x / (double)WIN_WIDTH - 1; // Normalized coordinate in camera space

    // Calculate ray direction
    Vector ray_dir = setVector(dir.x + plane.x * camera_x,
                               dir.y + plane.y * camera_x,
                               sqrt(ray_dir.x * ray_dir.x + ray_dir.y * ray_dir.y),
                               dir.angle);

    ray_dir = normalizeVector(ray_dir);
    rotateVector(&ray_dir, offset + x * rad_per_col);

    // Perform raycasting with ray_dir and player->actor.pos as origin
    // Example of calling intersection functions (to be implemented)
    Vector row_intersect = getRayRowIntersect(player->actor.pos, ray_dir, scene);
    Vector col_intersect = getRayColIntersect(player->actor.pos, ray_dir, scene);

    // Store the closest intersection or whatever you need
    player->actor.view_cone[x] = row_intersect.mag < col_intersect.mag ? row_intersect : col_intersect;
  }
}
