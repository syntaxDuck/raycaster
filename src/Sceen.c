#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stdbool.h>

#include "Actor.h"
#include "Sceen.h"
#include "Utility.h"

void loadSceen(Sceen *sceen, Uint8 **map) {

  sceen->map = malloc(sceen->height * sizeof(Uint8 *));
  for (int i = 0; i < sceen->height; i++) {
    sceen->map[i] = malloc(sceen->width * sizeof(Uint8));
  }

  for (int row = 0; row < sceen->width; row++) {
    for (int col = 0; col < sceen->width; col++) {
      sceen->map[row][col] = map[row][col];
    }
  }
}

void draw2DSceen(SDL_Renderer *renderer, Sceen sceen) {
  drawMap(renderer, sceen);
  drawPlayer(renderer, sceen.player);

  SDL_RenderPresent(renderer);
}

void drawMap(SDL_Renderer *renderer, Sceen sceen) {
  int col_offset;
  int row_offset;

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  for (int row = 0; row < sceen.height; row++) {
    if (row == sceen.height - 1)
      row_offset = 0;
    else
      row_offset = 1;

    for (int col = 0; col < sceen.width; col++) {

      if (col == sceen.width - 1)
        col_offset = 0;
      else
        col_offset = 1;

      if (sceen.map[row][col]) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      }

      SDL_Rect rectangle = {col * (sceen.unit_size), row * (sceen.unit_size),
                            sceen.unit_size - col_offset,
                            sceen.unit_size - row_offset};
      SDL_RenderFillRect(renderer, &rectangle);
    }
  }
}

void drawPlayer(SDL_Renderer *renderer, Player player) {
  drawActor(renderer, player.actor);
  drawActorViewDir(renderer, player.actor);
  drawActorVelDir(renderer, player.actor);
  drawActorViewRay(renderer, player.actor);
}

void drawActorViewDir(SDL_Renderer *renderer, Actor actor) {
  Vector view = transposeVector(actor.pos, actor.vect_view);
  SDL_RenderDrawLine(renderer, actor.pos.x, actor.pos.y, view.point.x,
                     view.point.y);
}

void drawActorVelDir(SDL_Renderer *renderer, Actor actor) {
  Vector vel = actor.vect_vel;
  scaleVector(&vel, 10);
  vel = transposeVector(actor.pos, vel);
  SDL_RenderDrawLine(renderer, actor.pos.x, actor.pos.y, vel.point.x,
                     vel.point.y);
}

void drawActorViewRay(SDL_Renderer *renderer, Actor actor) {
  // Vector ray = transposeVector(actor.pos, actor.view_ray);
  Vector ray = actor.view_ray;
  SDL_RenderDrawLine(renderer, actor.pos.x, actor.pos.y, ray.point.x,
                     ray.point.y);
}

void process2DSceen(Sceen *sceen) {
  processPlayerMotion(&sceen->player);
  processPlayerView(sceen);
}

void processPlayerMotion(Player *player) {

  const Uint8 *state = SDL_GetKeyboardState(NULL);

  // Rotation Function
  if (state[SDL_SCANCODE_RIGHT] ^ state[SDL_SCANCODE_LEFT]) {
    if (state[SDL_SCANCODE_LEFT]) {
      rotateVector(&player->actor.vect_view, -0.01);
    } else {
      rotateVector(&player->actor.vect_view, 0.01);
    }
    player->actor.vect_vel.angle = player->actor.vect_view.angle;
  }

  // Velocity Vector Function
  if (state[SDL_SCANCODE_UP] ^ state[SDL_SCANCODE_DOWN]) {
    if (state[SDL_SCANCODE_UP]) {
      player->actor.vect_vel.angle = player->actor.vect_view.angle;
    } else {
      player->actor.vect_vel.angle = player->actor.vect_view.angle + M_PI;
    }

    if ((player->actor.vect_vel.mag + player->actor.accel) >
        player->actor.max_vel) {
      rescaleVector(&player->actor.vect_vel, player->actor.max_vel);
    } else {
      scaleVector(&player->actor.vect_vel, player->actor.accel);
    }
  }

  else {
    rescaleVector(&player->actor.vect_vel, 0);
  }

  player->actor.pos =
      translatePoints(player->actor.pos, player->actor.vect_vel.point);
}

void processPlayerView(Sceen *sceen) {
  Actor player_actor = sceen->player.actor;
  Vector player_view_vect = sceen->player.actor.vect_view;

  double x, y;
  double row_x, row_y;
  double col_x, col_y;
  double row_mag, col_mag;
  int row_index, col_index;

  double player_cot = 1 / tan(player_view_vect.angle);
  int row_offset = ((int)player_actor.pos.y >> 6) << 6;
  while (true) {
    if (player_view_vect.angle == 0 || player_view_vect.angle == M_PI) {
      if (player_view_vect.angle == 0)
        x = 1000;
      else
        x = -1000;
      y = 0;

      row_x = player_actor.pos.x - x;
      row_y = player_actor.pos.y + y;
      row_mag = sqrt(x * x + y * y);
      break;
    }

    else {
      if (player_view_vect.angle > 0 && player_view_vect.angle < M_PI) {
        x = (player_actor.pos.y - row_offset) * player_cot;
        y = row_offset - player_actor.pos.y;

        row_offset -= 64;
        row_index = -1;

      } else {
        row_offset += 64;
        x = -(row_offset - player_actor.pos.y) * player_cot;
        y = row_offset - player_actor.pos.y;
        row_index = 0;
      }

      row_x = player_actor.pos.x - x;
      row_y = player_actor.pos.y + y;
      row_mag = sqrt(x * x + y * y);

      row_index += row_y / 64;
      col_index = row_x / 64;

      if (row_index < 0)
        row_index = 0;

      if (row_index >= sceen->height)
        row_index = sceen->height - 1;

      if (row_x < 0)
        break;

      if (row_x > sceen->width * 64)
        break;

      if (sceen->map[row_index][col_index] == 1)
        break;
    }
  }

  double player_tan = tan(player_view_vect.angle);
  int col_offset = ((int)player_actor.pos.x >> 6) << 6;
  while (true) {
    if (player_view_vect.angle == M_PI / 2 ||
        player_view_vect.angle == M_PI / 2 + M_PI) {

      if (player_view_vect.angle == M_PI / 2)
        y = 1000;
      else
        y = -1000;

      x = 0;

      col_x = player_actor.pos.x + x;
      col_y = player_actor.pos.y - y;
      col_mag = sqrt(x * x + y * y);
      break;
    }

    else {

      if (player_view_vect.angle < M_PI / 2 ||
          player_view_vect.angle > M_PI / 2 + M_PI) {
        x = col_offset - player_actor.pos.x;
        y = (player_actor.pos.x - col_offset) * player_tan;

        col_offset -= 64;
        col_index = -1;
      } else {
        col_offset += 64;
        x = col_offset - player_actor.pos.x;
        y = -(col_offset - player_actor.pos.x) * player_tan;
        col_index = 0;
      }

      col_x = player_actor.pos.x + x;
      col_y = player_actor.pos.y - y;
      col_mag = sqrt(x * x + y * y);

      row_index = col_y / 64;
      col_index += col_x / 64;

      if (col_index < 0)
        col_index = 0;

      if (col_index >= sceen->width)
        col_index = sceen->width - 1;

      if (col_y < 0)
        break;

      if (col_y > sceen->height * 64)
        break;

      if (sceen->map[row_index][col_index] == 1)
        break;
    }
  }

  if (row_mag < col_mag) {
    sceen->player.actor.view_ray.point.x = row_x;
    sceen->player.actor.view_ray.point.y = row_y;
    sceen->player.actor.view_ray.mag = row_mag;
  } else {
    sceen->player.actor.view_ray.point.x = col_x;
    sceen->player.actor.view_ray.point.y = col_y;
    sceen->player.actor.view_ray.mag = col_mag;
  }
}
