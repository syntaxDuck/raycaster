#include <SDL2/SDL.h>

#include "Actor.h"
#include "Sceen.h"

void draw2DSceen(SDL_Renderer *renderer, Sceen *sceen) {
  drawMap(renderer, sceen);
  drawActor(renderer, sceen->player.actor);

  SDL_RenderPresent(renderer);
}

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

void drawMap(SDL_Renderer *renderer, Sceen *sceen) {
  int col_offset;
  int row_offset;

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  for (int row = 0; row < sceen->height; row++) {
    if (row == sceen->height - 1)
      row_offset = 0;
    else
      row_offset = 1;

    for (int col = 0; col < sceen->width; col++) {

      if (col == sceen->width - 1)
        col_offset = 0;
      else
        col_offset = 1;

      if (sceen->map[row][col]) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      }

      SDL_Rect rectangle = {col * (sceen->unit_size), row * (sceen->unit_size),
                            sceen->unit_size - col_offset,
                            sceen->unit_size - row_offset};
      SDL_RenderFillRect(renderer, &rectangle);
    }
  }
}
