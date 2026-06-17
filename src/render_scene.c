#include "render_scene.h"
#include "SDL3/SDL_log.h"
#include "boundaries.h"
#include "constants.h"
#include "framebuffer_3D.h"
#include "player.h"
#include "raycast_2D.h"
#include "raycast_3D.h"
#include "stdlib.h"

struct Scene {
  SDL_Renderer *renderer;
  Ray *ray_array;
  Ray *base_ray_array;
  boundary_vec *boundary_array;
  Player player;
  framebuffer_3D *framebuffer_3D;
  scene_value_3D *sv;
  int num_of_rays;
  int num_of_boundaries;
  const bool *keys;
};

Scene *init_scene(SDL_Renderer *renderer, const bool *keys) {

  //---------------------Allocating memory for the Scene struct----------
  Scene *scene = calloc(1, sizeof(Scene));

  //------------------------------Initialization of Player---------
  init_player(&scene->player);

  //-------------------------------Initialization of boundaries-----------------
  int num_of_boundaries = NUM_OF_BOUNDARIES;
  scene->boundary_array = init_boundaries(BOUNDARY_WIDTH, BOUNDARY_HEIGHT);
  scene->num_of_boundaries = num_of_boundaries;
  // scene->boundary_array =
  //     init_random_boundaries(SCREEN_WIDTH_HALF, SCREEN_HEIGHT);

  //-------------------------------Initialization of Player Rays----------------

  int num_of_rays = NUM_OF_RAYS;
  scene->num_of_rays = num_of_rays;
  scene->ray_array = init_rays(num_of_rays, D_SPACING);
  scene->base_ray_array = init_rays(num_of_rays, D_SPACING);
  scene->renderer = renderer;
  scene->keys = keys;

  //-----------------------Initialization of the 3D Framebuffer---------------
  //
  framebuffer_3D *fb =
      init_framebuffer_3D(scene->renderer, VIEW_WIDTH, VIEW_HEIGHT,
                          VIEW_START_X, VIEW_START_Y, DRAW_WIDTH, DRAW_HEIGHT);
  if (fb == NULL) {
    SDL_Log("Framebuffer 3D initialization failed. Could not set up scene");
    return NULL;
  }
  scene->framebuffer_3D = fb;
  //

  //------------Initialization of the 3D scene values--------------------

  float half_width = (float)NUM_OF_RAYS / 2.0f;
  float half_fov_rad = ((float)FOV / 2.0f) * D_R;
  float tan_value = tanf(half_fov_rad);
  float dist_to_proj_plane = half_width / tan_value;
  float wall_height = 128.0f;
  float player_height = 64.0f;
  float center_y = DRAW_HEIGHT / 2.0f;

  scene_value_3D *sv =
      init_scene_3D(DRAW_WIDTH, DRAW_HEIGHT, wall_height, dist_to_proj_plane,
                    player_height, center_y);
  if (sv == NULL) {
    SDL_Log("3D Scene initalization failed. Could not set up scene");
    return NULL;
  }
  scene->sv = sv;
  //

  //---Returning pointer to the scene
  return scene;
}

void render_scene(Scene *scene, float dt) {

  Ray *ray_array = scene->ray_array;
  Ray *base_ray_array = scene->base_ray_array;
  Player *player = &scene->player;
  SDL_Renderer *renderer = scene->renderer;
  image_buffer *wall_src = &scene->framebuffer_3D->wall_tex;
  image_buffer *floor_src = &scene->framebuffer_3D->floor_tex;
  image_buffer *ceiling_src = &scene->framebuffer_3D->ceiling_tex;
  image_buffer *dest = &scene->framebuffer_3D->draw;
  scene_value_3D *scene_val_3D = scene->sv;
  boundary_vec *boundary_array = scene->boundary_array;
  const bool *keys = scene->keys;

  move_player_keys(player, keys, dt);

  update_player_rays(ray_array, base_ray_array, player->angle,
                     scene->num_of_rays);

  //-----------------------Draw 3D View Loop--------------------
  for (int i = 0; i < scene->num_of_rays; i++) {

    Vector2 player_pos = get_player_pos(player);

    intersect_data id =
        cast_single_ray(renderer, &ray_array[i], player_pos, boundary_array,
                        scene->num_of_boundaries);

    draw_floor_column(scene_val_3D, player_pos, floor_src, dest, &ray_array[i],
                      i);

    draw_ceiling_column(scene_val_3D, player_pos, ceiling_src, dest,
                        &ray_array[i], i);

    draw_wall_column(&id, scene_val_3D, i, dest, wall_src);
  }

  //------Making sure player does not clip through walls-----
  for (int i = 0; i < scene->num_of_boundaries; i++) {
    // draw_boundary(renderer, boundary_array, i);
    resolve_collision(player, boundary_array[i].start_vec,
                      boundary_array[i].end_vec);
  }

  // draw_player(renderer, player);
  if (!render_framebuffer_3D(scene->framebuffer_3D)) {
    SDL_Log("Rendering the framebuffer failed. L");
  }
}

void destroy_scene(Scene *scene) {
  SDL_Log("\n");
  destroy_ray_array(scene->base_ray_array);
  destroy_ray_array(scene->ray_array);
  destroy_scene_3D(scene->sv);
  destroy_framebuffer_3D(scene->framebuffer_3D);
  destroy_boundaries(scene->boundary_array);
  free(scene);
  SDL_Log("Scene has been destroyed");
  SDL_Log("\n");
}
