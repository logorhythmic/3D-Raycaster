#include "raycast_2D.h"
#include "SDL3/SDL_log.h"
#include "float.h"
#include "math.h"
#include "stdlib.h"
#include <SDL3/SDL_render.h>

#define D_R 0.0174533

Ray *init_rays(int num_of_rays, float degree_spacing) {
  float deg = -((num_of_rays - 1.0f) / 2.0f) * degree_spacing;
  Ray *ray_array = malloc(num_of_rays * sizeof(Ray));
  for (int i = 0; i < num_of_rays; i++) {
    ray_array[i].ray_dir = (Vector2){.x = cos(deg * D_R), .y = sin(deg * D_R)};
    ray_array[i].cos_beta = cos(deg * D_R);
    ray_array[i].beta = deg;
    ray_array[i].cos_alpha = 0.0f;
    ray_array[i].sin_alpha = 0.0f;
    deg += degree_spacing;
  }
  return ray_array;
}

float get_beta_deg(Ray *ray, int ray_num) { return ray[ray_num].beta; }

static inline void update_ray_angle_val(Ray *ray, float player_angle) {
  float alpha = player_angle + ray->beta;
  ray->cos_alpha = cos(alpha * D_R);
  ray->sin_alpha = sin(alpha * D_R);
}

// Rotates the base rays by current angle of player instead of recalculating
// rays every frame.
void update_player_rays(Ray *ray, Ray *corresponding_base_ray,
                        float current_player_angle, int num_of_rays) {
  float delta_cos = cos(current_player_angle * D_R);
  float delta_sin = sin(current_player_angle * D_R);

  // Using the rotation formula
  for (int i = 0; i < num_of_rays; i++) {

    // Rotation matrix formula
    ray[i].ray_dir.x = corresponding_base_ray[i].ray_dir.x * delta_cos -
                       corresponding_base_ray[i].ray_dir.y * delta_sin;

    ray[i].ray_dir.y = corresponding_base_ray[i].ray_dir.x * delta_sin +
                       corresponding_base_ray[i].ray_dir.y * delta_cos;

    update_ray_angle_val(&ray[i], current_player_angle);
  }
}

// Checking if ray intersects by checking intersection of single ray will all
// boundaries
// Solved Using paramteric line equation formula
intersect_data paramteric_calc_intersection(Vector2 player_ray,
                                            Vector2 player_pos,
                                            boundary_vec boundary) {
  Vector2 start_vec = boundary.start_vec;
  Vector2 end_vec = boundary.end_vec;
  intersect_data final = {0};
  final.valid = false;
  Vector2 V = v_sub(start_vec, player_pos);
  Vector2 R = v_sub(end_vec, start_vec);
  Vector2 S = player_ray;
  float D = v_det(S, R);
  if (D == 0) {
    return final;
  }
  float D1 = v_det(V, R);
  float D2 = v_det(V, S);
  float t = D1 / D;
  float s = D2 / D;
  if (t > 0.0f && s > 0.0f && s <= 1.0f) {
    final.valid = true;
    final.intersect_point = v_add(player_pos, v_scale(S, t));
    final.ray_dist = t;
    final.dist_along_wall = s * boundary.length;
    final.dist_param_s = s;
    final.boundary = boundary;
    return final;
  };
  return final;
}

// Loop through all cast rays. Cast the ray that is has the smallest distance to
// the boundary.

intersect_data cast_single_ray(SDL_Renderer *renderer, Ray *ray,
                               Vector2 player_pos, boundary_vec *boundary_array,
                               int num_of_boundaries) {
  intersect_data result = {0};
  result.valid = false;
  float min_distance = FLT_MAX;
  for (int i = 0; i < num_of_boundaries; i++) {

    intersect_data intersect = paramteric_calc_intersection(
        v_normalize(ray->ray_dir), player_pos, boundary_array[i]);

    if (intersect.ray_dist < min_distance && intersect.valid) {
      min_distance = intersect.ray_dist;
      result = intersect;
    }
  }

  // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  // SDL_RenderLine(renderer, player_pos.x, player_pos.y,
  // result.intersect_point.x,
  //                result.intersect_point.y);
  result.cos_beta = ray->cos_beta;
  return result;
}

void destroy_ray_array(Ray *ray_array) {
  free(ray_array);
  SDL_Log("Destroyed ray array\n");
}
