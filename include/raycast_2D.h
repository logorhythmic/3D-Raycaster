#ifndef RAYCAST_2D_H
#define RAYCAST_2D_H
#include "boundaries.h"
#include "vector.h"

typedef struct {
  bool valid;
  Vector2 intersect_point;
  float ray_dist;
  boundary_vec boundary;
  float dist_along_wall;
  float dist_param_s;
  float cos_beta;
} intersect_data;

struct Ray {
  Vector2 ray_dir;
  float beta;
  float cos_beta;
  float sin_alpha;
  float cos_alpha;
};
typedef struct Ray Ray;

Ray *init_rays(int num_of_rays, float degree_spacing);

void update_player_rays(Ray *ray, Ray *corresponding_base_ray,
                        float current_player_angle, int num_of_rays);
float get_beta_deg(Ray *ray, int ray_num);
intersect_data cast_single_ray(SDL_Renderer *renderer, Ray *ray,
                               Vector2 player_pos, boundary_vec *boundary_array,
                               int num_of_boundaries);
void destroy_ray_array(Ray *ray_array);
#endif
