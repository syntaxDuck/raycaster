
#include <SDL2/SDL.h>

#include "Actor.h"
#include "Utility.h"

void processActorMotion(Actor *actor) {
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  int vect_velMag = sqrt(actor->vect_vel.x * actor->vect_vel.x +
                         actor->vect_vel.y * actor->vect_vel.y);

  // Horizontal vect_vel Function
  if (state[SDL_SCANCODE_RIGHT] ^ state[SDL_SCANCODE_LEFT]) {
    if (vect_velMag < actor->max_vel) {
      if (state[SDL_SCANCODE_LEFT]) {
        actor->vect_vel.x -= actor->accel;
      } else {
        actor->vect_vel.x += actor->accel;
      }
    }
  } else if (state[SDL_SCANCODE_RIGHT] && state[SDL_SCANCODE_LEFT]) {
    actor->vect_vel.x = 0;
  } else {
    // Gradual deceleration to stop
    if (actor->vect_vel.x > 0) {
      actor->vect_vel.x -= actor->accel;
      if (actor->vect_vel.x < 0)
        actor->vect_vel.x = 0;
    } else if (actor->vect_vel.x < 0) {
      actor->vect_vel.x += actor->accel;
      if (actor->vect_vel.x > 0)
        actor->vect_vel.x = 0;
    }
  }

  // Vertical vect_vel Function
  if (state[SDL_SCANCODE_UP] ^ state[SDL_SCANCODE_DOWN]) {
    if (vect_velMag < actor->max_vel) {
      if (state[SDL_SCANCODE_UP]) {
        actor->vect_vel.y -= actor->accel;
      } else {
        actor->vect_vel.y += actor->accel;
      }
    } else {
      if (state[SDL_SCANCODE_UP]) {
        actor->vect_vel.y -= actor->accel;
      } else {
        actor->vect_vel.y += actor->accel;
      }

      if (actor->vect_vel.x > 0) {
        actor->vect_vel.x -= actor->accel;
      } else {
        actor->vect_vel.x += actor->accel;
      }
    }
  } else if (state[SDL_SCANCODE_DOWN] && state[SDL_SCANCODE_UP]) {
    actor->vect_vel.y = 0;
  } else {
    // Gradual deceleration to stop
    if (actor->vect_vel.y > 0) {
      actor->vect_vel.y -= actor->accel;
      if (actor->vect_vel.y < 0)
        actor->vect_vel.y = 0;
    } else if (actor->vect_vel.y < 0) {
      actor->vect_vel.y += actor->accel;
      if (actor->vect_vel.y > 0)
        actor->vect_vel.y = 0;
    }
  }

  actor->vect_pos.x += actor->vect_vel.x;
  actor->vect_pos.y += actor->vect_vel.y;
}

void createActorViewCone(Actor *actor) {
  int x = actor->vect_pos.x, y = actor->vect_pos.y + 100;
  Vector viewPoint;
  Vector *cone = malloc(sizeof(Vector) * actor->FOV);

  for (int i = 0; i < actor->FOV; i++) {
    viewPoint.x = actor->vect_pos.x;
    viewPoint.y = actor->vect_pos.y + 100;
    rotateVector(actor->vect_pos.x, actor->vect_pos.y,
                 (-actor->FOV / 2 + 1) + i, &viewPoint);
    cone[i] = viewPoint;
  }

  actor->viewCone = cone;
}

void processActor(SDL_Renderer *renderer, Actor *actor) {
  processActorMotion(actor);

  createActorViewCone(actor);

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

  for (int i = 0; i < actor->FOV; i++) {
    SDL_RenderDrawLine(renderer, actor->vect_pos.x, actor->vect_pos.y,
                       actor->viewCone[i].x, actor->viewCone[i].y);
  }

  generateFilledCircle(renderer, actor->vect_pos, actor->size, 500);
  SDL_RenderDrawLine(renderer, actor->vect_pos.x, actor->vect_pos.y,
                     actor->vect_pos.x + 100 * actor->vect_vel.x,
                     actor->vect_pos.y + 100 * actor->vect_vel.y);
}
