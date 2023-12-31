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
  drawRays(renderer, player.actor);
}

void drawRays(SDL_Renderer *renderer, Actor actor) {
  SDL_RenderDrawLine(renderer, actor.vect_pos.x, actor.vect_pos.y, actor.ray.x,
                     actor.ray.y);
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
      player->actor.angle -= 0.5;
      if (player->actor.angle == 0)
        player->actor.angle = 360;
    } else {
      player->actor.angle += 0.5;
      if (player->actor.angle == 360)
        player->actor.angle = 0;
    }
  }

  // Velocity Vector Function
  if (state[SDL_SCANCODE_UP] ^ state[SDL_SCANCODE_DOWN]) {

    double velMag = sqrt(player->actor.vect_vel.x * player->actor.vect_vel.x +
                         player->actor.vect_vel.y * player->actor.vect_vel.y);

    if ((velMag + player->actor.accel) > player->actor.max_vel) {
      velMag = player->actor.max_vel;
    } else {
      velMag += player->actor.accel;
    }

    double rad;
    if (state[SDL_SCANCODE_UP]) {
      rad = player->actor.angle * M_PI / 180.0;
      player->actor.vect_vel.x = velMag * cos(rad);
      player->actor.vect_vel.y = velMag * sin(rad);
    } else {
      rad = (player->actor.angle + 180) * M_PI / 180.0;
      player->actor.vect_vel.x = velMag * cos(rad);
      player->actor.vect_vel.y = velMag * sin(rad);
    }
  } else {

    player->actor.vect_vel.y = 0;
    player->actor.vect_vel.x = 0;
  }

  player->actor.vect_pos.x += player->actor.vect_vel.x;
  player->actor.vect_pos.y += player->actor.vect_vel.y;
}

void processPlayerRays(Sceen *sceen) {
  double rad = sceen->player.actor.angle * M_PI / 180.0;
  sceen->player.actor.ray.x = sceen->player.actor.vect_pos.x +
                              sceen->player.actor.view_distance * cos(rad);
  sceen->player.actor.ray.y = sceen->player.actor.vect_pos.y +
                              sceen->player.actor.view_distance * sin(rad);
}
