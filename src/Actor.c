
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>

#include "Actor.h"
#include "Utility.h"

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

  actor->view_cone = cone;
}

void drawActor(SDL_Renderer *renderer, Actor actor) {
  // createActorViewCone(actor);

  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

  SDL_Rect rect = {(int)actor.vect_pos.x, (int)actor.vect_pos.y, actor.size,
                   actor.size};

  SDL_RenderFillRect(renderer, &rect);

  // for (int i = 0; i < actor->FOV; i++) {
  //   SDL_RenderDrawLine(renderer, actor->vect_pos.x, actor->vect_pos.y,
  //                      actor->view_cone[i].x, actor->view_cone[i].y);
  // }
  //
  // generateFilledCircle(renderer, actor->vect_pos, actor->size, 500);
  // SDL_RenderDrawLine(renderer, actor->vect_pos.x, actor->vect_pos.y,
  //                    actor->vect_pos.x + 100 * actor->vect_vel.x,
  //                    actor->vect_pos.y + 100 * actor->vect_vel.y);
}
