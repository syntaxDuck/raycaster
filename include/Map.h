#ifndef MAP_H
#define MAP_H

#include <SDL2/SDL.h>
#include <stdint.h>

#include "Defines.h"

typedef struct
{
    Uint8 **walls;
    Uint8 **ceil;
    Uint8 **floor;
    int unit_size;
    int width;
    int height;
} Map;

Map loadMap(char *filename);
void printMap(Map map);
void loadMapGridFromFile(const char *filename, Map *map);
void freeMap(Map map);

#endif