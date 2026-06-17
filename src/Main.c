#include "Main.h"
#include "SDL3/SDL_hints.h"
#include "constants.h"
#include "render_scene.h"
#include "stdio.h"
#include "stdlib.h"
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <stdint.h>

#define S_NS 1000000000 /* Second to nanosecond */
int main() {
  printf("Hello World. This is working\n");
  SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);

  //---------Making a window-----------
  SDL_Window *window;
  SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_SCALE_TO_DISPLAY, "1");
  window =
      SDL_CreateWindow("Sample SDL3 window", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  printf("Logical Size: %dx%d\n", w, h);

  SDL_GetWindowSizeInPixels(window, &w, &h);
  printf("Physical Size (Pixels): %dx%d\n", w, h);

  float scale = SDL_GetWindowDisplayScale(window);
  printf("Display Scale: %f\n", scale);

  //----Initalize software renderer
  SDL_Renderer *renderer;
  renderer = SDL_CreateRenderer(window, NULL);

  if (!SDL_SetRenderVSync(renderer, 1)) {
    SDL_Log("\n WARNING: VSYNC NOT SUPPORTED\n");
  }

  //---------------------Initializing the scene----------------
  const bool *keys = SDL_GetKeyboardState(NULL);
  Scene *scene = init_scene(renderer, keys);
  if (scene == NULL) {
    SDL_Log("Scene initialization failed. Exiting program");
    exit(0);
  }

  //----------Frame Rate and Delta Time Calculation-----------------------
  int frame_count = 0;
  int total_frame_count = 0;
  double total_frame_time = 0.0;
  char title[64];

  double inv_performance_frequency =
      (double)1.0 / SDL_GetPerformanceFrequency();
  double delta_time = 0;
  Uint64 last_dt = SDL_GetPerformanceCounter();
  Uint64 current_dt = 0;
  Uint64 frame_start_tick = 0;
  Uint64 frame_end_tick = 0;
  Uint64 current_fps_tick;
  Uint64 last_fps_tick = SDL_GetPerformanceCounter();

  //-------------------------Main Loop-------------------------
  bool done = false;
  SDL_Event event;

  while (!done) {

    frame_start_tick = SDL_GetPerformanceCounter();

    //---------------Delta time calculation for movement--------------
    current_dt = frame_start_tick;
    delta_time = (double)(current_dt - last_dt) * inv_performance_frequency;
    last_dt = current_dt;
    //----------------------------------------------

    //----------------Event Checking---------------------
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        done = true;
      }
    }
    //----------------------------------------------------

    //----------------Main frame----------------------------------
    SDL_RenderClear(renderer);
    render_scene(scene, delta_time);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
    //---------------------------------------------------

    frame_end_tick = SDL_GetPerformanceCounter();

    //----Calculation to display average frame time at the end-------
    double frame_time =
        (double)(frame_end_tick - frame_start_tick) * inv_performance_frequency;
    total_frame_count++;
    total_frame_time += frame_time;
    //-------------------------------------------------

    frame_count++;

    current_fps_tick = SDL_GetPerformanceCounter();
    double fps_elapsed_seconds =
        (double)(current_fps_tick - last_fps_tick) * inv_performance_frequency;

    if (fps_elapsed_seconds >= 1) {
      snprintf(title, sizeof(title), "FPS: %d ", frame_count);
      SDL_SetWindowTitle(window, title);
      frame_count = 0;
      last_fps_tick = current_fps_tick;
    }
  }
  SDL_Log("\nAverage time per frame: %lf ms",
          1000 * ((double)total_frame_time / total_frame_count));
  SDL_Log("\n");
  SDL_DestroyRenderer(renderer);
  destroy_scene(scene);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
