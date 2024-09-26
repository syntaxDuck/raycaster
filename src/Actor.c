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
      translatePoints(actor->pos, actor->velocity.point);
}

Vector getRayRowIntersect(Point origin, Vector ray, Scene scene)
{
  Vector casted_ray;
  double x, y;
  int row_index, col_index;

  double ray_cot = 1 / tan(ray.angle);
  int row_offset = ((int)origin.y >> 6) << 6;
  while (true)
  {
    if (ray.angle == 0 || ray.angle == M_PI)
    {
      x = ray.angle == 0 ? 1000 : -1000;
      y = 0;

      casted_ray.point.x = origin.x - x;
      casted_ray.point.y = origin.y + y;
      casted_ray.mag = sqrt(x * x + y * y);
      break;
    }

    else
    {
      if (ray.angle > 0 && ray.angle < M_PI)
      {
        x = (origin.y - row_offset) * ray_cot;
        y = row_offset - origin.y;

        row_offset -= 64;
        row_index = -1;
      }
      else
      {
        row_offset += 64;
        x = -(row_offset - origin.y) * ray_cot;
        y = row_offset - origin.y;
        row_index = 0;
      }

      casted_ray.point.x = origin.x - x;
      casted_ray.point.y = origin.y + y;
      casted_ray.mag = sqrt(x * x + y * y);

      row_index += casted_ray.point.y / 64;
      col_index = casted_ray.point.x / 64;

      if (row_index < 0)
        row_index = 0;

      if (row_index >= scene.height)
        row_index = scene.height - 1;

      if (casted_ray.point.x < 0)
        break;

      if (casted_ray.point.x > scene.width * 64)
        break;

      if (scene.map[row_index][col_index] == 1)
      {
        break;
      }
    }
  }
  return casted_ray;
}

Vector getRayColIntersect(Point origin, Vector ray, Scene scene)
{
  Vector casted_ray;
  double x, y;
  int row_index, col_index;

  double ray_tan = tan(ray.angle);
  int col_offset = ((int)origin.x >> 6) << 6;
  while (true)
  {
    if (ray.angle == M_PI_2 ||
        ray.angle == M_PI_3_2)
    {

      y = ray.angle == M_PI_2 ? 1000 : -1000;
      x = 0;

      casted_ray.point.x = origin.x + x;
      casted_ray.point.y = origin.y - y;
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

        col_offset -= 64;
        col_index = -1;
      }
      else
      {
        col_offset += 64;
        x = col_offset - origin.x;
        y = -(col_offset - origin.x) * ray_tan;
        col_index = 0;
      }

      casted_ray.point.x = origin.x + x;
      casted_ray.point.y = origin.y - y;
      casted_ray.mag = sqrt(x * x + y * y);

      row_index = casted_ray.point.y / 64;
      col_index += casted_ray.point.x / 64;

      if (col_index < 0)
        col_index = 0;

      if (col_index >= scene.width)
        col_index = scene.width - 1;

      if (casted_ray.point.y < 0)
        break;

      if (casted_ray.point.y > scene.height * 64)
        break;

      if (scene.map[row_index][col_index] == 1)
      {
        break;
      }
    }
  }
  return casted_ray;
}

void castActorRays(Actor *actor, Scene scene)
{
  double increment_rad = actor->field_of_view / actor->number_of_rays;
  double starting_angle = -actor->field_of_view / 2 + increment_rad;
  for (int i = 0; i < actor->number_of_rays; i++)
  {
    Vector new_vect = actor->dir;
    rotateVector(&new_vect,
                 starting_angle + i * increment_rad);

    Vector row_intersect = getRayRowIntersect(actor->pos, new_vect, scene);
    Vector col_intersect = getRayColIntersect(actor->pos, new_vect, scene);
    actor->view_cone[i] = row_intersect.mag < col_intersect.mag ? row_intersect : col_intersect;
  }
}