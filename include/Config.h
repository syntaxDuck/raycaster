#ifndef CONFIG_H
#define CONFIG_H

#include "window.h"
// #include "player.h"

// Env defines
#define TEX_WIDTH 64
#define TEX_HEIGHT 64
#define REND_2D true
#define REND_FP true
#define DEBUG
#define TEXTURES_PNG true

#define DEG_TO_RAD M_PI / 180

// Actor defines
#define NUM_RAYS 200

#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define MAP_UNIT_SIZE 20

#define CONFIG_PATH "config.ini"
#define DEFAULT_WIN_TITLE "Game Window"
#define DEFAULT_WIN_WIDTH (MAP_UNIT_SIZE * MAP_WIDTH)
#define DEFAULT_WIN_HEIGHT (MAP_UNIT_SIZE * MAP_HEIGHT)
#define DEFAULT_WIN_MAX_FPS 120

typedef struct
{
    WindowConfig *window_config;
    // PlayerConfig player_config;
} Config;

Config *init_config();
void print_config();

#endif