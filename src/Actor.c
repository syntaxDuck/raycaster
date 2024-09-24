
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <stdbool.h>

#include "Scene.h"
#include "Actor.h"
#include "Utility.h"

// void createActorViewCone(Actor *actor) {
//   int x = actor->vect_pos.x, y = actor->vect_pos.y + 100;
//   Vector viewPoint;
//   Vector *cone = malloc(sizeof(Vector) * actor->FOV);
//
//   for (int i = 0; i < actor->FOV; i++) {
//     viewPoint.x = actor->vect_pos.x;
//     viewPoint.y = actor->vect_pos.y + 100;
//     rotateVector(actor->vect_pos.x, actor->vect_pos.y,
//                  (-actor->FOV / 2 + 1) + i, &viewPoint);
//     cone[i] = viewPoint;
//   }
//
//   actor->view_cone = cone;
// }

void processActorView(Actor *actor, Scene *scene)
{
  Vector view_vect = actor->vect_view;

  double x, y;
  double row_x, row_y;
  double col_x, col_y;
  double row_mag, col_mag;
  int row_index, col_index;

  double player_cot = 1 / tan(view_vect.angle);
  int row_offset = ((int)actor->pos.y >> 6) << 6;
  while (true)
  {
    if (view_vect.angle == 0 || view_vect.angle == M_PI)
    {
      if (view_vect.angle == 0)
        x = 1000;
      else
        x = -1000;
      y = 0;

      row_x = actor->pos.x - x;
      row_y = actor->pos.y + y;
      row_mag = sqrt(x * x + y * y);
      break;
    }

    else
    {
      if (view_vect.angle > 0 && view_vect.angle < M_PI)
      {
        x = (actor->pos.y - row_offset) * player_cot;
        y = row_offset - actor->pos.y;

        row_offset -= 64;
        row_index = -1;
      }
      else
      {
        row_offset += 64;
        x = -(row_offset - actor->pos.y) * player_cot;
        y = row_offset - actor->pos.y;
        row_index = 0;
      }

      row_x = actor->pos.x - x;
      row_y = actor->pos.y + y;
      row_mag = sqrt(x * x + y * y);

      row_index += row_y / 64;
      col_index = row_x / 64;

      if (row_index < 0)
        row_index = 0;

      if (row_index >= scene->height)
        row_index = scene->height - 1;

      if (row_x < 0)
        break;

      if (row_x > scene->width * 64)
        break;

      if (scene->map[row_index][col_index] == 1)
        break;
    }
  }

  double player_tan = tan(view_vect.angle);
  int col_offset = ((int)actor->pos.x >> 6) << 6;
  while (true)
  {
    if (view_vect.angle == M_PI / 2 ||
        view_vect.angle == M_PI / 2 + M_PI)
    {

      if (view_vect.angle == M_PI / 2)
        y = 1000;
      else
        y = -1000;

      x = 0;

      col_x = actor->pos.x + x;
      col_y = actor->pos.y - y;
      col_mag = sqrt(x * x + y * y);
      break;
    }

    else
    {

      if (view_vect.angle < M_PI / 2 ||
          view_vect.angle > M_PI / 2 + M_PI)
      {
        x = col_offset - actor->pos.x;
        y = (actor->pos.x - col_offset) * player_tan;

        col_offset -= 64;
        col_index = -1;
      }
      else
      {
        col_offset += 64;
        x = col_offset - actor->pos.x;
        y = -(col_offset - actor->pos.x) * player_tan;
        col_index = 0;
      }

      col_x = actor->pos.x + x;
      col_y = actor->pos.y - y;
      col_mag = sqrt(x * x + y * y);

      row_index = col_y / 64;
      col_index += col_x / 64;

      if (col_index < 0)
        col_index = 0;

      if (col_index >= scene->width)
        col_index = scene->width - 1;

      if (col_y < 0)
        break;

      if (col_y > scene->height * 64)
        break;

      if (scene->map[row_index][col_index] == 1)
        break;
    }
  }

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
}