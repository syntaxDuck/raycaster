#ifndef SCEEN_H
#define SCEEN_H

#include "Actor.h"

typedef struct {
  Uint8 **map;
  Uint8 width;
  Uint8 height;
  Actor *actors;
} Sceen;

void loadSceen(Sceen *sceen, Uint8 **map);

#endif
