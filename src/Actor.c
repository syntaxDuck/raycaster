
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <stdbool.h>

#include "Scene.h"
#include "Actor.h"
#include "Utility.h"

// void createActorViewCone(Actor *actor)
// {
//   int x = actor->pos.x, y = actor->pos.y + 100;
//   Vector viewPoint;
//   Vector *cone = malloc(sizeof(Vector) * actor->FOV);

//   for (int i = 0; i < actor->FOV; i++)
//   {
//     viewPoint.point.x = actor->pos.x;
//     viewPoint.point.y = actor->pos.y + 100;
//     rotateVector(actor->pos.x, actor->pos.y,
//                  (-actor->FOV / 2 + 1) + i, &viewPoint);
//     cone[i] = viewPoint;
//   }

//   actor->view_cone = cone;
// }

void processActorMotion(Actor *actor)
{

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  // Rotation Function
  if (state[SDL_SCANCODE_RIGHT] ^ state[SDL_SCANCODE_LEFT])
  {
    if (state[SDL_SCANCODE_LEFT])
    {
      rotateVector(&actor->vect_view, -0.005);
    }
    else
    {
      rotateVector(&actor->vect_view, 0.005);
    }
    actor->vect_vel.angle = actor->vect_view.angle;
  }

  // Velocity Vector Function
  if (state[SDL_SCANCODE_UP] ^ state[SDL_SCANCODE_DOWN])
  {
    if (state[SDL_SCANCODE_UP])
    {
      actor->vect_vel.angle = actor->vect_view.angle;
    }
    else
    {
      actor->vect_vel.angle = actor->vect_view.angle + M_PI;
    }

    if ((actor->vect_vel.mag + actor->accel) >
        actor->max_vel)
    {
      rescaleVector(&actor->vect_vel, actor->max_vel);
    }
    else
    {
      scaleVector(&actor->vect_vel, actor->accel);
    }
  }

  else
  {
    rescaleVector(&actor->vect_vel, 0);
  }

  actor->pos =
      translatePoints(actor->pos, actor->vect_vel.point);
}

Vector getRayRowIntersect(Point origin, Vector ray, Scene scene)
{
  Vector casted_ray;
  double x, y;
  double row_x, row_y;
  double col_x, col_y;
  double row_mag;
  int row_index, col_index;

  double ray_cot = 1 / tan(ray.angle);
  int row_offset = ((int)origin.y >> 6) << 6;
  while (true)
  {
    if (ray.angle == 0 || ray.angle == M_PI)
    {
      if (ray.angle == 0)
        x = 1000;
      else
        x = -1000;
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

      if (col_index < 0)
        col_index = 0;

      if (col_index >= scene.width)
        col_index = scene.width - 1;

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
  double row_x, row_y;
  double col_x, col_y;
  double col_mag;
  int row_index, col_index;

  double ray_tan = tan(ray.angle);
  int col_offset = ((int)origin.x >> 6) << 6;
  while (true)
  {
    if (ray.angle == M_PI / 2 ||
        ray.angle == 3 * M_PI / 2)
    {

      if (ray.angle == M_PI / 2)
        y = 1000;
      else
        y = -1000;

      x = 0;

      casted_ray.point.x = origin.x + x;
      casted_ray.point.y = origin.y - y;
      casted_ray.mag = sqrt(x * x + y * y);
      break;
    }

    else
    {

      if (ray.angle < M_PI / 2 ||
          ray.angle > 3 * M_PI / 2)
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

      if (row_index < 0)
        row_index = 0;

      if (row_index >= scene.height)
        row_index = scene.height - 1;

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
        printf("x: %f, y: %f, row:%d, col:%d\n", casted_ray.point.x, casted_ray.point.y, row_index, col_index);
        break;
      }
    }
  }
  return casted_ray;
}

void processActorView(Actor *actor, Scene scene)
{

  Vector view_vect = actor->vect_view;

  double row_x, row_y;
  double col_x, col_y;
  double row_mag, col_mag;

  Vector new = getRayRowIntersect(actor->pos, view_vect, scene);
  Vector new1 = getRayColIntersect(actor->pos, view_vect, scene);
  row_x = new.point.x;
  row_y = new.point.y;
  row_mag = new.mag;
  col_x = new1.point.x;
  col_y = new1.point.y;
  col_mag = new1.mag;

  if (row_mag < col_mag)
  {
    actor->view_ray.point.x = row_x;
    actor->view_ray.point.y = row_y;
    actor->view_ray.mag = row_mag;
  }
  else
  {
    actor->view_ray.point.x = col_x;
    actor->view_ray.point.y = col_y;
    actor->view_ray.mag = col_mag;
  }
  printf("x: %f, y: %f\n", actor->view_ray.point.x, actor->view_ray.point.y);
}