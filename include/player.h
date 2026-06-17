#ifndef PLAYER_H
#define PLAYER_H

#include "vector.h"

#include "SDL3/SDL_render.h"
#include "SDL3/SDL_scancode.h"

typedef struct {
  Vector2 pos;
  Vector2 dir;
  float angle; // In degrees
  float move_len;
  float angle_change;
  float player_rad;
} Player;

void init_player(Player *player);
void move_player_keys(Player *p, const bool *keys, float dt);
void draw_player(SDL_Renderer *renderer, Player *p);
void resolve_collision(Player *player, Vector2 p1, Vector2 p2);
Vector2 get_player_pos(Player *player);
float get_player_angle(Player *player);

#endif
