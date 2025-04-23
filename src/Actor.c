#include "actor.h"
#include "config.h"
#include <SDL.h>
#include <SDL_surface.h>
#include <stdbool.h>

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923
#endif

void process_actor_motion(Actor *actor, float frame_time, Map map)
{
  float movement_speed = 5 * map.unit_size * frame_time;

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  // Rotation Function
  if (state[SDL_SCANCODE_D] ^ state[SDL_SCANCODE_A])
  {
    if (state[SDL_SCANCODE_A])
    {
      rotate_vector(&actor->dir, -PLAYER_TURN_SPEED);
    }
    else
    {
      rotate_vector(&actor->dir, PLAYER_TURN_SPEED);
    }
    actor->velocity.angle = actor->dir.angle;
  }

  // Velocity Vector Function
  if (state[SDL_SCANCODE_W] ^ state[SDL_SCANCODE_S])
  {
    if (state[SDL_SCANCODE_W])
    {
      Vector new_pos = set_vector(actor->pos.x + actor->dir.x * movement_speed,
                                  actor->pos.y + actor->dir.y * movement_speed);
      if (map.walls[(int)actor->pos.y / map.unit_size]
                   [(int)new_pos.x / map.unit_size] == false)
        actor->pos.x = new_pos.x;
      if (map.walls[(int)new_pos.y / map.unit_size]
                   [(int)actor->pos.x / map.unit_size] == false)
        actor->pos.y = new_pos.y;
    }
    else
    {
      Vector new_pos = set_vector(actor->pos.x - actor->dir.x * movement_speed,
                                  actor->pos.y - actor->dir.y * movement_speed);
      if (map.walls[(int)actor->pos.y / map.unit_size]
                   [(int)new_pos.x / map.unit_size] == false)
        actor->pos.x = new_pos.x;
      if (map.walls[(int)new_pos.y / map.unit_size]
                   [(int)actor->pos.x / map.unit_size] == false)
        actor->pos.y = new_pos.y;
      return;
    }
  }
}

Vector get_ray_row_intersectect(Vector origin, Vector ray, Map map)
{
  Vector casted_ray;
  double x, y;
  int row_index, col_index;

  double ray_cot = 1 / tan(ray.angle);
  int row_offset = origin.y - (int)origin.y % map.unit_size;
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

        row_offset -= map.unit_size;
        row_index = -1;
      }
      else
      {
        row_offset += map.unit_size;
        x = -(row_offset - origin.y) * ray_cot;
        y = row_offset - origin.y;
        row_index = 0;
      }

      casted_ray.x = origin.x - x;
      casted_ray.y = origin.y + y;
      casted_ray.mag = sqrt(x * x + y * y);
      casted_ray.angle = ray.angle;

      row_index += casted_ray.y / map.unit_size;
      col_index = casted_ray.x / map.unit_size;

      if (row_index < 0)
        row_index = 0;

      if (row_index >= map.height)
        row_index = map.height - 1;

      if (casted_ray.x < 0)
        break;

      if (casted_ray.x > map.width * map.unit_size)
        break;

      if (map.walls[row_index][col_index] > 0)
      {
        break;
      }
    }
  }
  return casted_ray;
}

Vector get_ray_col_intersect(Vector origin, Vector ray, Map map)
{
  Vector casted_ray;
  double x, y;
  int row_index, col_index;

  double ray_tan = tan(ray.angle);
  int col_offset = origin.x - (int)origin.x % map.unit_size;
  while (true)
  {
    if (ray.angle == M_PI_2 || ray.angle == M_PI_3_2)
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

      if (ray.angle < M_PI_2 || ray.angle > M_PI_3_2)
      {
        x = col_offset - origin.x;
        y = (origin.x - col_offset) * ray_tan;

        col_offset -= map.unit_size;
        col_index = -1;
      }
      else
      {
        col_offset += map.unit_size;
        x = col_offset - origin.x;
        y = -(col_offset - origin.x) * ray_tan;
        col_index = 0;
      }

      casted_ray.x = origin.x + x;
      casted_ray.y = origin.y - y;
      casted_ray.mag = sqrt(x * x + y * y);
      casted_ray.angle = ray.angle;

      row_index = casted_ray.y / map.unit_size;
      col_index += casted_ray.x / map.unit_size;

      if (col_index < 0)
        col_index = 0;

      if (col_index >= map.width)
        col_index = map.width - 1;

      if (casted_ray.y < 0)
        break;

      if (casted_ray.y > map.height * map.unit_size)
        break;

      if (map.walls[row_index][col_index] > 0)
      {
        break;
      }
    }
  }
  return casted_ray;
}

void cast_actor_rays(Actor *actor, Map map)
{
  double increment_rad = actor->field_of_view / NUM_RAYS;
  double starting_angle = -actor->field_of_view / 2 + increment_rad;
  for (int i = 0; i < NUM_RAYS; i++)
  {
    Vector new_vect = actor->dir;
    rotate_vector(&new_vect, starting_angle + i * increment_rad);

    Vector row_intersect = get_ray_row_intersectect(actor->pos, new_vect, map);
    Vector col_intersect = get_ray_col_intersect(actor->pos, new_vect, map);
    actor->view_cone[i] =
        row_intersect.mag < col_intersect.mag ? row_intersect : col_intersect;
  }
}
