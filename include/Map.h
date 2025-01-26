#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include <stdint.h>

typedef struct {
  Uint8 **walls;
  Uint8 **ceil;
  Uint8 **floor;
  int unit_size;
  int width;
  int height;
} Map;

Map load_map(char *filename);
void print_map(Map map);
void load_map_grid_from_file(const char *filename, Map *map);
void free_map(Map map);

#endif
