#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <math.h>

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

void process2DSceen(Sceen *sceen) {
  processPlayerMotion(&sceen->player);
  processPlayerRays(sceen);
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

void processPlayerRays(Sceen *sceen) {
  Actor player_actor = sceen->player.actor;

  double x, y;
  double row_mag, col_mag;

  // double player_cot = 1 / tan(player_actor.angle);
  // int row_offset = ((int)player_actor.vect_pos.y >> 6) << 6;
  // if (player_actor.angle == 0 || player_actor.angle == M_PI) {
  //   if (player_actor.angle == 0)
  //     x = 128;
  //   else
  //     x = -128;
  //   y = player_actor.ray.y;
  // } else {
  //   if (player_actor.angle > 0 && player_actor.angle < M_PI) {
  //     x = (player_actor.vect_pos.y - row_offset) * player_cot;
  //     y = row_offset;
  //   } else {
  //     x = -(row_offset + 64 - player_actor.vect_pos.y) * player_cot;
  //     y = row_offset + 64;
  //   }
  // }
  //
  // sceen->player.actor.ray.x = player_actor.vect_pos.x - x;
  // sceen->player.actor.ray.y = y;
  //
  // row_mag = sqrt(x * x + y * y);

  // double player_tan = tan(player_actor.angle);
  // int col_offset = ((int)player_actor.vect_pos.x >> 6) << 6;
  // if (player_actor.angle == M_PI / 2 || player_actor.angle == M_PI / 2 +
  // M_PI) {
  //   x = player_actor.vect_pos.x;
  //
  //   if (player_actor.angle == M_PI / 2)
  //     y = 128;
  //   else
  //     y = -128;
  //
  // } else {
  //   if (player_actor.angle < M_PI_2 || player_actor.angle > M_PI_2 + M_PI) {
  //     x = col_offset;
  //     y = (player_actor.vect_pos.x - col_offset) * player_tan;
  //   } else {
  //     x = col_offset + 64;
  //     y = -(x - player_actor.vect_pos.x) * player_tan;
  //   }
  // }
  //
  // sceen->player.actor.ray.x = x;
  // sceen->player.actor.ray.y = player_actor.vect_pos.y - y;

  // sceen->player.actor.ray.x =
  //     sceen->player.actor.vect_pos.x +
  //     sceen->player.actor.view_distance * cos(sceen->player.actor.angle);
  // sceen->player.actor.ray.y =
  //     sceen->player.actor.vect_pos.y +
  //     sceen->player.actor.view_distance * sin(sceen->player.actor.angle);
}
