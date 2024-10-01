#ifndef CONFIG_H
#define CONFIG_H

// Env defines
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define MAP_UNIT_SIZE 20
#define TEX_WIDTH 64
#define TEX_HEIGHT 64
#define WIN_WIDTH (MAP_UNIT_SIZE * MAP_WIDTH)
#define WIN_HEIGHT (MAP_UNIT_SIZE * MAP_HEIGHT)
#define MAX_FPS 120
#define REND_2D false
#define REND_FP true
#define DEBUG true

// Generic
#define DEG_TO_RAD M_PI / 180
#define M_PI_3_2 M_PI + M_PI_2

// Player defines
#define PLAYER_SIZE 7
#define PLAYER_ACCEL 0.01
#define PLAYER_TURN_SPEED 0.05
#define PLAYER_MAX_SPEED 1
#define PLAYER_FOV 60

// Actor defines
#define NUM_RAYS 200

int textures[8][TEX_HEIGHT * TEX_WIDTH];

#endif