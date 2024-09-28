#ifndef CONFIG_H
#define CONFIG_H

// Env defines
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define MAP_UNIT_SIZE 20
#define WIN_WIDTH (MAP_UNIT_SIZE * MAP_WIDTH)
#define WIN_HEIGHT (MAP_UNIT_SIZE * MAP_HEIGHT)
#define REND_2D true
#define REND_FP false
#define DEBUG true

// Generic
#define DEG_TO_RAD M_PI / 180
#define M_PI_3_2 M_PI + M_PI_2

// Player defines
#define PLAYER_SIZE 7
#define PLAYER_ACCEL 0.05
#define PLAYER_TURN_SPEED 0.005
#define PLAYER_MAX_SPEED 1
#define PLAYER_FOV 66

// Actor defines
#define NUM_RAYS 200

#endif