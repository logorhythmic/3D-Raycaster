#ifndef FRAMEBUFFER_3D_H
#define FRAMEBUFFER_3D_H

#include "SDL3/SDL_render.h"
#include "stdbool.h"
#include <SDL3/SDL_surface.h>

typedef struct {
  uint32_t *pixels;
  int width;
  int height;
  int pitch;
} image_buffer;

typedef struct asset_pointers asset_pointers;

struct framebuffer_3D {
  int view_start_x;
  int view_start_y;
  int view_width;
  int view_height;
  SDL_Renderer *renderer;
  image_buffer draw;
  image_buffer wall_tex;
  image_buffer floor_tex;
  image_buffer ceiling_tex;
  SDL_Texture *raycast_3D_texture;
  asset_pointers *asset_pointers;
};

typedef struct framebuffer_3D framebuffer_3D;

/*
 * Initializes the 3D raycasting environment
 * (Texture assets, GPU Texture and the draw buffer)
 *
 * Returns: A valid framebuffer_3D pointer, or NULL if
 * allocation/SDL fails.
 *
 * Note: Caller is responsible for passing this to
 * destroy_framebuffer_3D() when done.
 */
framebuffer_3D *init_framebuffer_3D(SDL_Renderer *renderer, int view_width,
                                    int view_height, int view_start_x,
                                    int view_start_y, int draw_width,
                                    int draw_height);

/*
 * Renders the framebuffer and displays to the screen
 *
 * Returns: True if rendering occurered successfully, or false if rendering
 * fails.
 */
bool render_framebuffer_3D(framebuffer_3D *buffer);

void destroy_framebuffer_3D(framebuffer_3D *buffer);

#endif
