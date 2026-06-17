#include "player.h"
#include "constants.h"
#include "vector.h"
#include <SDL3/SDL_log.h>

void init_player(Player *player) {
  Vector2 player_pos = {.x = (float)SCREEN_WIDTH / 4.0f,
                        .y = SCREEN_HEIGHT - 1.0f};
  Vector2 player_dir = {.x = 1.0f, .y = 0.0f};
  float player_rad = 15.0f;
  int move_len = 300.0f;
  float angle = 0.0f;
  float angle_change = 90.0f;

  *player = (Player){
      .pos = player_pos,
      .dir = player_dir,
      .angle = angle,
      .angle_change = angle_change,
      .move_len = move_len,
      .player_rad = player_rad,
  };
}

float get_player_angle(Player *p) { return p->angle; }

Vector2 get_player_pos(Player *p) { return p->pos; };

void move_player_keys(Player *p, const bool *keys, float dt) {

  if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]) {
    p->angle += p->angle_change * dt;
    if (p->angle > 360.0f) {

      p->angle -= 360.0f;
    }
    p->dir.x = cos(p->angle * D_R);
    p->dir.y = sin(p->angle * D_R);
  }

  if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]) {
    p->angle -= p->angle_change * dt;
    if (p->angle < 0) {
      p->angle += 360.0f;
    }
    p->dir.x = cos(p->angle * D_R);
    p->dir.y = sin(p->angle * D_R);
  }

  if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP]) {
    p->pos = v_add(p->pos, v_scale(p->dir, p->move_len * dt));
  }

  if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN]) {
    p->pos = v_add(p->pos, v_scale(p->dir, -(p->move_len * dt)));
  }
}

void resolve_collision(Player *player, Vector2 p1, Vector2 p2) {

  Vector2 player_pos = player->pos;
  float radius = player->player_rad;

  Vector2 wall_dir = v_sub(p2, p1);
  float wall_length_sq = v_len_sq(wall_dir);

  float t = v_dot(v_sub(player_pos, p1), wall_dir) / wall_length_sq;
  t = SDL_clamp(t, 0.0f, 1.0f);

  // Point on wall line that circle intersects at
  Vector2 wall_intersect_point = v_add(p1, v_scale(wall_dir, t));

  // Vector of player point from point of intersection on wall line. That is
  // direction and it needs to be pushed bacl
  Vector2 push_dir = v_sub(player_pos, wall_intersect_point);
  float push_dist = v_mag(push_dir);
  if (push_dist < radius && push_dist > 0.0f) {
    float overlap = radius - push_dist;
    Vector2 push_dir_normal = v_normalize(push_dir);

    // Push player out by overlap
    player_pos = v_add(player_pos, v_scale(push_dir_normal, overlap));
    player->pos = player_pos;
  }
}

// void draw_player(SDL_Renderer *renderer, Player *p) {
//   // DrawRectangle(player_x, player_y, 50, 50, RED);
//   SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//   DrawCircleFilled(renderer, p->pos.x, p->pos.y, p->player_rad);
//   Vector2 end = v_add(v_scale(p->dir, 10.0f), p->pos);
//   SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//   SDL_RenderLine(renderer, p->pos.x, p->pos.y, end.x, end.y);
// }
