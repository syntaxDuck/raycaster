#include <SDL2/SDL.h>

#include "Sceen.h"

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
