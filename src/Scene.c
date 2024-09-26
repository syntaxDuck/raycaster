#include "Scene.h"

SDL_Renderer *renderer;

void setSceneRenderer(SDL_Renderer *rend)
{
  renderer = rend;
}

void loadScene(Scene *Scene, Uint8 map[MAP_HEIGHT][MAP_WIDTH])
{
  Scene->map = malloc(Scene->height * sizeof(Uint8 *));
  for (int i = 0; i < Scene->height; i++)
  {
    Scene->map[i] = malloc(Scene->width * sizeof(Uint8));
  }

  // Copy map data into Scene->map
  for (int row = 0; row < Scene->height; row++)
  {
    for (int col = 0; col < Scene->width; col++)
    {
      Scene->map[row][col] = map[row][col];
    }
  }
}

void draw2DScene(Scene Scene)
{
  drawMap(Scene);
  drawPlayer(Scene.player);

  SDL_RenderPresent(renderer);
}

void drawMap(Scene Scene)
{
  int col_offset;
  int row_offset;

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  for (int row = 0; row < Scene.height; row++)
  {
    if (row == Scene.height - 1)
      row_offset = 0;
    else
      row_offset = 1;

    for (int col = 0; col < Scene.width; col++)
    {

      if (col == Scene.width - 1)
        col_offset = 0;
      else
        col_offset = 1;

      if (Scene.map[row][col])
      {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      }
      else
      {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      }

      SDL_Rect rectangle = {col * (Scene.unit_size), row * (Scene.unit_size),
                            Scene.unit_size - col_offset,
                            Scene.unit_size - row_offset};
      SDL_RenderFillRect(renderer, &rectangle);
    }
  }
}

void drawPlayer(Player player)
{
  drawActor(player.actor);
  drawActorViewDir(player.actor);
  drawActorVelDir(player.actor);
  drawActorViewRays(player.actor);
}

void drawActor(Actor actor)
{
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

  SDL_Rect rect = {actor.pos.x - (actor.size >> 1),
                   actor.pos.y - (actor.size >> 1), actor.size, actor.size};

  SDL_RenderFillRect(renderer, &rect);
}

void drawActorViewDir(Actor actor)
{
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  Vector view = transposeVector(actor.pos, actor.dir);
  SDL_RenderDrawLine(renderer, actor.pos.x, actor.pos.y, view.point.x,
                     view.point.y);
}

void drawActorVelDir(Actor actor)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  Vector vel = actor.velocity;
  scaleVector(&vel, 10);
  vel = transposeVector(actor.pos, vel);
  SDL_RenderDrawLine(renderer, actor.pos.x, actor.pos.y, vel.point.x,
                     vel.point.y);
}

void drawActorViewRays(Actor actor)
{
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 1);
  for (int i = 0; i < actor.number_of_rays; i++)
  {
    Vector ray = actor.view_cone[i];
    SDL_RenderDrawLine(renderer, actor.pos.x, actor.pos.y, ray.point.x,
                       ray.point.y);
  }
}

void drawVectorFromActor(Actor actor, Vector vect, int scale, long color)
{
  SDL_SetRenderDrawColor(renderer, color >> 24, color >> 16 & 0xFF, 255 >> 8 & 0xFF, color & 0xFF);
  scaleVector(&vect, scale);
  vect = transposeVector(actor.pos, vect);
  SDL_RenderDrawLine(renderer, actor.pos.x, actor.pos.y, vect.point.x, vect.point.y);
}

void process2DScene(Scene *scene)
{
  Actor *playerActor = &scene->player.actor;
  processActorMotion(playerActor);
  castActorRays(playerActor, *scene);
}