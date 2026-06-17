#ifndef BOUNDARIES_H
#define BOUNDARIES_H

#include "vector.h"
#include <SDL3/SDL_render.h>

struct boundary_vec {
  Vector2 start_vec;
  Vector2 end_vec;
  float length;
};
typedef struct boundary_vec boundary_vec;

/* Initalizes the hardcoded boundary values
 * Returns: Pointer to the boundary vector struct
 * Note: Caller is responsible for calling destroy_boundaries() after use
 */
boundary_vec *init_boundaries(int screen_width, int screen_height);

void draw_boundary(SDL_Renderer *renderer, boundary_vec *boundary,
                   int boundary_number);

/* Initalizes random boundary values within the bounds of screen
 * Returns: Pointer to the boundary vector struct
 * Note: Caller is responsible for calling destroy boundaries after use
 */
boundary_vec *init_random_boundaries(int screen_width, int screen_height);

void destroy_boundaries(boundary_vec *boundary);

#endif
