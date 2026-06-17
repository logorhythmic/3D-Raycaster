#ifndef RAYCAST_3D_H
#define RAYCAST_3D_H
#include "framebuffer_3D.h"
#include "raycast_2D.h"

typedef struct scene_value_3D scene_value_3D;

typedef struct angle_values angle_values;

/* Initalizes the scene valeus for the 3D environment
 * Returns: A pointer to scene_value_3D struct
 * Note: Caller is responsible for calling destroy_scene_3D after use
 */
scene_value_3D *init_scene_3D(int draw_buffer_height, int draw_buffer_width,
                              int wall_height, float dist_to_proj_plane,
                              float player_height, float center_y);

void update_angle_values(scene_value_3D *sv, float beta_start,
                         float player_angle);

void draw_wall_column(intersect_data *id, scene_value_3D *cam, int wall_x,
                      image_buffer *draw_buffer, image_buffer *wall_buffer);

void draw_floor_column(const scene_value_3D *cam, Vector2 player_pos,
                       image_buffer *src, image_buffer *dest, Ray *ray,
                       int dest_x);

void draw_ceiling_column(const scene_value_3D *cam, Vector2 player_pos,
                         image_buffer *src, image_buffer *dest, Ray *ray,
                         int wall_x);

void destroy_scene_3D(scene_value_3D *sv);

#endif
