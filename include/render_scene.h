#ifndef RENDER_SCENE_H
#define RENDER_SCENE_H
#include "raycast_2D.h"

typedef struct Scene Scene;

/* Initializes the entire scene
 * Returns: A valid Scene pointer or NULL if allocation fails
 * Note: Caller is responible for passing the pointer to destroy_scene() when
 * done
 */
Scene *init_scene(SDL_Renderer *renderer, const bool *keys);

void render_scene(Scene *scene, float dt);

/* Destroys the scene by freeing the Scene struct
 *
 */
void destroy_scene(Scene *scene);

#endif
