#ifndef CONFIG_H
#define CONFIG_H

// Env defines
#define MAP_WIDTH 8
#define MAP_HEIGHT 8
#define MAP_UNIT_SIZE 64
#define WIN_WIDTH (MAP_UNIT_SIZE * MAP_WIDTH)
#define WIN_HEIGHT (MAP_UNIT_SIZE * MAP_HEIGHT)

// Player defines
#define PLAYER_SIZE 7
#define PLAYER_ACCEL 0.05
#define PLAYER_TURN_SPEED 0.005
#define PLAYER_MAX_SPEED 1
#define PLAYER_FOV 90

#endif