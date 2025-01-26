#include "player.h"
#include "config.h"
#include "global.h"

void castPlayerRays(Player *player, Map map) {
  Vector dir = player->actor.dir; // Player's direction vector
  Vector plane =
      player->plane; // Player's plane vector (perpendicular to direction)
  Vector pos = setVector(player->actor.pos.x / map.unit_size,
                         player->actor.pos.y / map.unit_size);

  double rad_per_col =
      player->actor.field_of_view / win_ctx->width; // Radians per pixel
  double offset = -player->actor.field_of_view / 2;
  double camera_x;

  free(player->intersects);
  player->intersects = malloc(sizeof(WallIntersect) * win_ctx->width);

  for (int x = 0; x < win_ctx->width; x++) {
    // Calculate x-coordinate in camera space
    camera_x = 2 * x / (double)win_ctx->width -
               1; // Normalized coordinate in camera space
    Vector ray_dir =
        setVector(dir.x + plane.x * camera_x, dir.y + plane.y * camera_x);
    player->intersects[x] = getWallIntersect(pos, ray_dir, map);
    player->actor.view_cone[x] = player->intersects[x].vect;
  }
}

WallIntersect getWallIntersect(Vector origin, Vector ray_dir, Map map) {
  Vector map_pos = setVector((int)origin.x, (int)origin.y);
  Vector side_dist;
  Vector delta_dist = setVector(ray_dir.x == 0 ? 1e30 : fabs(1 / ray_dir.x),
                                ray_dir.y == 0 ? 1e30 : fabs(1 / ray_dir.y));
  double perp_wall_dist;
  Vector step;
  int hit = 0;
  int side;
  if (ray_dir.x < 0) {
    step.x = -1;
    side_dist.x = (origin.x - map_pos.x) * delta_dist.x;
  } else {
    step.x = 1;
    side_dist.x = (map_pos.x + 1.0 - origin.x) * delta_dist.x;
  }

  if (ray_dir.y < 0) {
    step.y = -1;
    side_dist.y = (origin.y - map_pos.y) * delta_dist.y;
  } else {
    step.y = 1;
    side_dist.y = (map_pos.y + 1.0 - origin.y) * delta_dist.y;
  }

  while (hit == 0) {
    if (side_dist.x < side_dist.y) {
      side_dist.x += delta_dist.x;
      map_pos.x += step.x;
      side = 0;
    } else {
      side_dist.y += delta_dist.y;
      map_pos.y += step.y;
      side = 1;
    }

    if (map.walls[(int)map_pos.y][(int)map_pos.x] > 0)
      hit = 1;
  }
  if (side == 0)
    perp_wall_dist = side_dist.x - delta_dist.x;
  else
    perp_wall_dist = side_dist.y - delta_dist.y;

  WallIntersect intersect;
  intersect.vect = setVector(origin.x + ray_dir.x * perp_wall_dist,
                             origin.y + ray_dir.y * perp_wall_dist);
  intersect.perp_wall_distance = perp_wall_dist;
  intersect.side = side;
  intersect.map_x = map_pos.x;
  intersect.map_y = map_pos.y;
  intersect.ray_dir = ray_dir;
  return intersect;
}

void processPlayerMotion(Player *player, float fps, Map map) {
  processActorMotion(&player->actor, 1 / fps, map);
  rotateVector(&player->plane,
               player->actor.dir.angle - player->plane.angle + M_PI_2);
  castPlayerRays(player, map);
}

Player createPlayer() {
  Player player;

  player.actor.size = PLAYER_SIZE;
  player.actor.field_of_view = PLAYER_FOV * DEG_TO_RAD;

  // Allocate memory for the player's view cone
  player.actor.view_cone = malloc(sizeof(Vector) * win_ctx->width);
  player.actor.max_vel = PLAYER_MAX_SPEED;
  player.actor.accel = PLAYER_ACCEL;

  // Initialize player's position and velocity
  player.actor.pos = setVector(2 * (double)win_ctx->width / 3 - 1,
                               2 * (double)win_ctx->height / 3 - 1);
  player.actor.velocity = setVector(0, 0);
  player.actor.dir = setVector(-1, 0);

  // Calculate the player's plane (used for field of view in 3D rendering)
  player.plane =
      setVector(0, player.actor.dir.x * tan(player.actor.field_of_view / 2));
  player.intersects = malloc(sizeof(WallIntersect) * win_ctx->width);

  return player;
}

void freePlayer(Player *player) {
  free(player->actor.view_cone);
  free(player->intersects);
}
