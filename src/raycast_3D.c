#include "raycast_3D.h"
#include "constants.h"
#include "framebuffer_3D.h"
#include "raycast_2D.h"
#include "stdlib.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <math.h>
#include <stdalign.h>
#include <stdint.h>
#include <sys/types.h>
#define D_R 0.0174533

typedef struct {
  int start_y;
  int end_y;
  float line_height;
  float param_s;
  float dist_along_wall;
  int draw_x;
  float tex_step;
  float beta;
} column_info;

struct angle_values {
  float sin_ray_angle;
  float cos_ray_angle;
  float cos_beta;
};

struct scene_value_3D {
  int screen_width_2d;
  int draw_buffer_width;
  int draw_buffer_height;
  int num_rays;
  float fov;
  float degree_spacing;     // Derived from FOV / num_rays
  float dist_to_proj_plane; // Derived from (screen_width / 2) / tan(FOV / 2)
  float wall_height;        // Base scalar for wall rendering
  float player_height;
  int center_y;
  angle_values angle_values;
};

scene_value_3D *init_scene_3D(int draw_buffer_height, int draw_buffer_width,
                              int wall_height, float dist_to_proj_plane,
                              float player_height, float center_y) {
  scene_value_3D *sv = malloc(sizeof(scene_value_3D));
  sv->draw_buffer_width = draw_buffer_width;
  sv->draw_buffer_width = draw_buffer_height;
  sv->wall_height = wall_height;
  sv->dist_to_proj_plane = dist_to_proj_plane;
  sv->player_height = player_height;
  sv->center_y = center_y;
  return sv;
}

void destroy_scene_3D(scene_value_3D *sv) { free(sv); }

column_info obtain_column_info(intersect_data *intersect_data, int wall_x,
                               scene_value_3D *cam) {

  // Correction applied to remove fisheye effect
  float cos_beta = intersect_data->cos_beta;
  float min_distance = intersect_data->ray_dist * cos_beta;
  if (min_distance <= 0.0f)
    min_distance = 0.001f;

  float line_height =
      (cam->wall_height / min_distance) * cam->dist_to_proj_plane;

  float start_y = cam->center_y - (line_height / 2.0f);

  int end_y = start_y + line_height;

  return (column_info){.start_y = (int)start_y,
                       .end_y = (int)end_y,
                       .line_height = line_height,
                       .param_s = intersect_data->dist_param_s,
                       .draw_x = wall_x,
                       .beta = intersect_data->cos_beta,
                       .dist_along_wall = intersect_data->dist_along_wall};
}

void draw_textured_column(image_buffer *draw_buffer, image_buffer *wall_buffer,
                          column_info *ci) {
  int true_start_y = ci->start_y;
  int true_end_y = ci->end_y;
  int dest_x = ci->draw_x;
  int colmn_start_y = true_start_y;
  int colmn_end_y = true_end_y;

  if (true_start_y < 0) {
    colmn_start_y = 0;
  }

  if (true_end_y > draw_buffer->height) {
    colmn_end_y = draw_buffer->height;
  }

  // Using linear interpolation to draw textures to the column

  float tile_count = ci->dist_along_wall / wall_buffer->width;
  float s = tile_count - floorf(tile_count);
  int src_tex_x = floorf(s * wall_buffer->width);

  float step_y = (float)(wall_buffer->height) / (true_end_y - true_start_y);

  float tex_pos_y = (colmn_start_y - true_start_y) * step_y;

  if (tex_pos_y < 0 || tex_pos_y > wall_buffer->height) {
    SDL_Log("Error\n");
  }

  for (int y = colmn_start_y; y < colmn_end_y; y++) {

    int src_tex_y = (int)(tex_pos_y);

    int src_tex = src_tex_y * wall_buffer->width + src_tex_x;
    int des_tex = y * draw_buffer->width + dest_x;
    tex_pos_y += step_y;

    draw_buffer->pixels[des_tex] = wall_buffer->pixels[src_tex];
  }
}

void draw_wall_column(intersect_data *id, scene_value_3D *cam, int wall_x,
                      image_buffer *draw_buffer, image_buffer *wall_buffer) {

  column_info ci = obtain_column_info(id, wall_x, cam);
  draw_textured_column(draw_buffer, wall_buffer, &ci);
}

void draw_floor_column(const scene_value_3D *cam, Vector2 player_pos,
                       image_buffer *src, image_buffer *dest, Ray *ray,
                       int wall_x) {

  float player_height = cam->player_height;
  int dest_x = wall_x;
  int centre = cam->center_y;
  int screen_height = dest->height;
  float dist_to_proj_plane = cam->dist_to_proj_plane;

  uint32_t *src_pixels = src->pixels;
  uint32_t *dest_pixels = dest->pixels;

  int src_tex_width = src->width;
  int src_tex_height = src->height;

  float cos_beta = ray->cos_beta;
  float cos_alpha = ray->cos_alpha;
  float sin_alpha = ray->sin_alpha;

  float interm = (player_height * dist_to_proj_plane) / cos_beta;

  // Future Ref: Check Permadi Tutorial for floor and ceiling casting
  for (int i = centre + 1; i < screen_height; i++) {

    float actual_dist = interm / (float)(i - centre);
    float world_x = player_pos.x + actual_dist * cos_alpha;
    float world_y = player_pos.y + actual_dist * sin_alpha;

    float tile_size = 128.0f;
    if (THEME == 0) {
      tile_size = 256.0f;
    }

    int src_x =
        (world_x / tile_size - floorf(world_x / tile_size)) * src_tex_width;
    int src_y =
        (world_y / tile_size - floorf(world_y / tile_size)) * src_tex_height;

    src_x = SDL_clamp(src_x, 0, src_tex_width - 1);
    src_y = SDL_clamp(src_y, 0, src_tex_height - 1);

    int src_index = src_y * src_tex_width + src_x;
    int dest_index = i * dest->width + dest_x;

    dest_pixels[dest_index] = src_pixels[src_index];
  }
}

void draw_ceiling_column(const scene_value_3D *cam, Vector2 player_pos,
                         image_buffer *src, image_buffer *dest, Ray *ray,
                         int wall_x) {

  float player_height = cam->player_height;
  int dest_x = wall_x;
  int centre = cam->center_y;
  float dist_to_proj_plane = cam->dist_to_proj_plane;

  uint32_t *src_pixels = src->pixels;
  uint32_t *dest_pixels = dest->pixels;

  int src_tex_width = src->width;
  int src_tex_height = src->height;

  float cos_beta = ray->cos_beta;
  float cos_alpha = ray->cos_alpha;
  float sin_alpha = ray->sin_alpha;

  float interm = (player_height * dist_to_proj_plane) / cos_beta;

  // Future Ref: Check Permadi Tutorial for floor and ceiling casting
  for (int i = centre - 1; i >= 0; i--) {

    float actual_dist = interm / (float)(centre - i);
    float world_x = player_pos.x + actual_dist * cos_alpha;
    float world_y = player_pos.y + actual_dist * sin_alpha;

    float tile_size = 512.0f;

    int src_x =
        (world_x / tile_size - floorf(world_x / tile_size)) * src_tex_width;

    int src_y =
        (world_y / tile_size - floorf(world_y / tile_size)) * src_tex_height;

    src_x = SDL_clamp(src_x, 0, src_tex_width - 1);
    src_y = SDL_clamp(src_y, 0, src_tex_height - 1);

    int src_index = src_y * src_tex_width + src_x;
    int dest_index = i * dest->width + dest_x;

    dest_pixels[dest_index] = src_pixels[src_index];
  }
}
