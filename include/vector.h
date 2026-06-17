#ifndef VECTOR_H
#define VECTOR_H
#include "math.h"
#include <stdbool.h>

typedef struct Vector2 {
  float x;
  float y;
} Vector2;

static inline Vector2 v_add(Vector2 a, Vector2 b) {
  return (Vector2){a.x + b.x, a.y + b.y};
}

// Vector A - Vector  B
static inline Vector2 v_sub(Vector2 a, Vector2 b) {
  return (Vector2){a.x - b.x, a.y - b.y};
}

static inline float v_mag(Vector2 a) {
  return sqrtf((a.x * a.x) + (a.y * a.y));
}

static inline Vector2 v_scale(Vector2 a, float scale) {

  return (Vector2){a.x * scale, a.y * scale};
}

static inline float v_len_sq(Vector2 a) { return (a.x * a.x) + (a.y * a.y); }

static inline float v_dot(Vector2 a, Vector2 b) {
  return (a.x * b.x + a.y * b.y);
}

static inline Vector2 v_normalize(Vector2 a) {
  float mag = sqrt((a.x * a.x + a.y * a.y));

  if (mag > 0) {
    return (Vector2){a.x / mag, a.y / mag};
  }

  return (Vector2){0, 0};
}

static inline float v_det(Vector2 a, Vector2 b) {
  return (a.x * b.y - a.y * b.x);
}

static inline bool v_same(Vector2 a, Vector2 b) {
  if (a.x == b.x && a.y == b.y) {
    return true;
  }
  return false;
}

// static inline Color scale_color(Color col, double scale) {
//   Color new_col = {0, 0, 0, 0};
//   new_col.r = scale * col.r;
//   new_col.g = scale * col.g;
//   new_col.b = scale * col.b;
//   new_col.a = col.a;
//
//   return new_col;
// }

#endif
