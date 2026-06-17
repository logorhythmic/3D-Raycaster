#include "boundaries.h"
#include "constants.h"
#include <SDL3/SDL_render.h>
#include <stdlib.h>

boundary_vec *init_boundaries(int screen_width, int screen_height) {

  int num_of_boundaries = NUM_OF_BOUNDARIES;

  boundary_vec *boundary_array =
      malloc(num_of_boundaries * sizeof(boundary_vec));

  // Pillar 1 (X: 250 -> 450)
  boundary_array[0].start_vec = (Vector2){.x = 250.0f, .y = 275.0f};
  boundary_array[0].end_vec = (Vector2){.x = 450.0f, .y = 275.0f}; // Top
  boundary_array[1].start_vec = (Vector2){.x = 450.0f, .y = 275.0f};
  boundary_array[1].end_vec = (Vector2){.x = 450.0f, .y = 525.0f}; // Right
  boundary_array[2].start_vec = (Vector2){.x = 450.0f, .y = 525.0f};
  boundary_array[2].end_vec = (Vector2){.x = 250.0f, .y = 525.0f}; // Bottom
  boundary_array[3].start_vec = (Vector2){.x = 250.0f, .y = 525.0f};
  boundary_array[3].end_vec = (Vector2){.x = 250.0f, .y = 275.0f}; // Left

  // Pillar 2 (X: 700 -> 900)
  boundary_array[4].start_vec = (Vector2){.x = 700.0f, .y = 275.0f};
  boundary_array[4].end_vec = (Vector2){.x = 900.0f, .y = 275.0f}; // Top
  boundary_array[5].start_vec = (Vector2){.x = 900.0f, .y = 275.0f};
  boundary_array[5].end_vec = (Vector2){.x = 900.0f, .y = 525.0f}; // Right
  boundary_array[6].start_vec = (Vector2){.x = 900.0f, .y = 525.0f};
  boundary_array[6].end_vec = (Vector2){.x = 700.0f, .y = 525.0f}; // Bottom
  boundary_array[7].start_vec = (Vector2){.x = 700.0f, .y = 525.0f};
  boundary_array[7].end_vec = (Vector2){.x = 700.0f, .y = 275.0f}; // Left

  // Pillar 3 (X: 1150 -> 1350)
  boundary_array[8].start_vec = (Vector2){.x = 1150.0f, .y = 275.0f};
  boundary_array[8].end_vec = (Vector2){.x = 1350.0f, .y = 275.0f}; // Top
  boundary_array[9].start_vec = (Vector2){.x = 1350.0f, .y = 275.0f};
  boundary_array[9].end_vec = (Vector2){.x = 1350.0f, .y = 525.0f}; // Right
  boundary_array[10].start_vec = (Vector2){.x = 1350.0f, .y = 525.0f};
  boundary_array[10].end_vec = (Vector2){.x = 1150.0f, .y = 525.0f}; // Bottom
  boundary_array[11].start_vec = (Vector2){.x = 1150.0f, .y = 525.0f};
  boundary_array[11].end_vec = (Vector2){.x = 1150.0f, .y = 275.0f}; // Left

  // Manually initliazing the screen border vectors
  Vector2 top_right = (Vector2){.x = screen_width, .y = 0};
  Vector2 top_left = (Vector2){.x = 0, .y = 0};
  Vector2 bottom_right =
      (Vector2){.x = screen_width - 1, .y = screen_height - 1};
  Vector2 bottom_left = (Vector2){.x = 0 + 1, .y = screen_height - 1};

  // 1. Top Border
  boundary_array[num_of_boundaries - 4].start_vec = top_left;
  boundary_array[num_of_boundaries - 4].end_vec = top_right;

  // 2. Bottom Border
  boundary_array[num_of_boundaries - 3].start_vec = bottom_left;
  boundary_array[num_of_boundaries - 3].end_vec = bottom_right;

  // 3. Left Border
  boundary_array[num_of_boundaries - 2].start_vec = top_right;
  boundary_array[num_of_boundaries - 2].end_vec = bottom_right;

  // 4. Right Border
  boundary_array[num_of_boundaries - 1].start_vec = top_left;
  boundary_array[num_of_boundaries - 1].end_vec = bottom_left;

  for (int i = 0; i < num_of_boundaries; i++) {
    boundary_array[i].length =
        v_mag(v_sub(boundary_array[i].end_vec, boundary_array[i].start_vec));
  }
  return boundary_array;
}

boundary_vec *init_random_boundaries(int screen_width, int screen_height) {
  SDL_srand(0);
  int num_of_boundaries = NUM_OF_BOUNDARIES;

  boundary_vec *boundary_array =
      malloc(num_of_boundaries * sizeof(boundary_vec));
  for (int i = 0; i < num_of_boundaries - 4; i++) {

    boundary_array[i].start_vec =
        (Vector2){.x = (float)SDL_rand(screen_width),
                  .y = (float)SDL_rand(screen_height)};

    boundary_array[i].end_vec = (Vector2){.x = (float)SDL_rand(screen_width),
                                          .y = (float)SDL_rand(screen_height)};
  }

  Vector2 top_right = (Vector2){.x = screen_width, .y = 0};
  Vector2 top_left = (Vector2){.x = 0, .y = 0};
  Vector2 bottom_right =
      (Vector2){.x = screen_width - 1, .y = screen_height - 1};
  Vector2 bottom_left = (Vector2){.x = 0 + 1, .y = screen_height - 1};

  // 1. Top Border
  boundary_array[num_of_boundaries - 4].start_vec = top_left;
  boundary_array[num_of_boundaries - 4].end_vec = top_right;

  // 2. Bottom Border
  boundary_array[num_of_boundaries - 3].start_vec = bottom_left;
  boundary_array[num_of_boundaries - 3].end_vec = bottom_right;

  // 3. Left Border
  boundary_array[num_of_boundaries - 2].start_vec = top_right;
  boundary_array[num_of_boundaries - 2].end_vec = bottom_right;

  // 4. Right Border
  boundary_array[num_of_boundaries - 1].start_vec = top_left;
  boundary_array[num_of_boundaries - 1].end_vec = bottom_left;

  for (int i = 0; i < num_of_boundaries; i++) {
    boundary_array[i].length =
        v_mag(v_sub(boundary_array[i].end_vec, boundary_array[i].start_vec));
  }
  return boundary_array;
}

void draw_boundary(SDL_Renderer *renderer, boundary_vec *boundary,
                   int boundary_number) {
  boundary_vec boundary_array = boundary[boundary_number];
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderLine(renderer, boundary_array.start_vec.x,
                 boundary_array.start_vec.y, boundary_array.end_vec.x,
                 boundary_array.end_vec.y);
}

void destroy_boundaries(boundary_vec *boundary) { free(boundary); }
