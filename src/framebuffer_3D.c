#include "framebuffer_3D.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_surface.h"
#include "constants.h"
#include "stdlib.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <stdint.h>
#include <sys/types.h>

typedef struct asset_pointers {
  SDL_Surface *wall_surface;
  SDL_Surface *floor_surface;
  SDL_Surface *ceiling_surface;
} asset_pointers;

bool init_draw_buffer(framebuffer_3D *fb) {
  int draw_width = fb->draw.width;
  int draw_height = fb->draw.height;

  if (draw_width == 0) {
    SDL_Log("WARNING WARNING");
  }
  uint32_t *pixel_buffer = calloc(draw_width * draw_height, sizeof(uint32_t));

  if (pixel_buffer == NULL) {
    SDL_Log("Malloc failed to allocate memory for pixel buffer");
    return false;
  }
  fb->draw.pixels = pixel_buffer;
  return true;
}

bool init_raycast_3D_texture(framebuffer_3D *buffer) {
  int w = buffer->draw.width;
  int h = buffer->draw.height;
  SDL_Texture *raycast_3D_texture =
      SDL_CreateTexture(buffer->renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, w, h);
  if (raycast_3D_texture == NULL) {
    SDL_Log("Failed to create 3D raycast texture: %s", SDL_GetError());
    return false;
  }

  buffer->raycast_3D_texture = raycast_3D_texture;
  SDL_SetTextureScaleMode(buffer->raycast_3D_texture, SDL_SCALEMODE_LINEAR);
  return true;
}

bool init_wall_buffer(framebuffer_3D *buffer) {
  SDL_Surface *original_surface;
  if (THEME == 0) {
    original_surface = SDL_LoadPNG("assets/light/Wall.png");
  }
  if (THEME == 1) {
    original_surface = SDL_LoadPNG("assets/dark/Wall.png");
  }
  if (THEME == 2) {
    original_surface = SDL_LoadPNG("assets/cartoon/Wall.png");
  }

  if (!original_surface) {
    SDL_Log("Failed to load wall texture: %s", SDL_GetError());
    return false;
  }
  int width = original_surface->w;
  int height = original_surface->h;
  SDL_Log("Wall Texture -> Width: %d, Height: %d\n", width, height);

  SDL_Surface *wall_surface =
      SDL_ConvertSurface(original_surface, SDL_PIXELFORMAT_ARGB8888);

  if (!wall_surface) {
    SDL_Log("Failed to convert wall surface: %s", SDL_GetError());
    return false;
  }
  buffer->asset_pointers->wall_surface = wall_surface;
  buffer->wall_tex.height = wall_surface->h;
  buffer->wall_tex.width = wall_surface->w;
  buffer->wall_tex.pixels = wall_surface->pixels;
  buffer->wall_tex.pitch = wall_surface->pitch / sizeof(uint32_t);
  SDL_DestroySurface(original_surface);
  // SDL_DestroySurface(wall_surface);
  return true;
}

bool init_floor_buffer(framebuffer_3D *buffer) {

  SDL_Surface *original_surface;
  if (THEME == 0) {
    original_surface = SDL_LoadPNG("assets/light/Floor.png");
  }
  if (THEME == 1) {
    original_surface = SDL_LoadPNG("assets/dark/Floor.png");
  }
  if (THEME == 2) {
    original_surface = SDL_LoadPNG("assets/cartoon/Floor.png");
  }

  int width = original_surface->w;
  int height = original_surface->h;
  SDL_Log("Floor Texture -> Width: %d, Height: %d\n", width, height);

  SDL_Surface *floor_surface =
      SDL_ConvertSurface(original_surface, SDL_PIXELFORMAT_ARGB8888);

  if (!floor_surface) {
    SDL_Log("Failed to convert wall surface: %s", SDL_GetError());
    return false;
  }
  buffer->asset_pointers->floor_surface = floor_surface;
  buffer->floor_tex.height = floor_surface->h;
  buffer->floor_tex.width = floor_surface->w;
  buffer->floor_tex.pixels = floor_surface->pixels;
  buffer->floor_tex.pitch = floor_surface->pitch / sizeof(uint32_t);
  SDL_DestroySurface(original_surface);
  return true;
}

bool init_ceiling_buffer(framebuffer_3D *buffer) {

  SDL_Surface *original_surface;
  if (THEME == 0) {
    original_surface = SDL_LoadPNG("assets/light/Ceiling.png");
  }
  if (THEME == 1) {
    original_surface = SDL_LoadPNG("assets/dark/Ceiling.png");
  }
  if (THEME == 2) {
    original_surface = SDL_LoadPNG("assets/cartoon/Ceiling.png");
  }

  int width = original_surface->w;
  int height = original_surface->h;
  SDL_Log("Ceiling Texture -> Width: %d, Height: %d\n", width, height);

  SDL_Surface *ceiling_surface =
      SDL_ConvertSurface(original_surface, SDL_PIXELFORMAT_ARGB8888);

  if (!ceiling_surface) {
    SDL_Log("Failed to convert wall surface: %s", SDL_GetError());
    return false;
  }
  buffer->asset_pointers->ceiling_surface = ceiling_surface;
  buffer->ceiling_tex.height = ceiling_surface->h;
  buffer->ceiling_tex.width = ceiling_surface->w;
  buffer->ceiling_tex.pixels = ceiling_surface->pixels;
  buffer->ceiling_tex.pitch = ceiling_surface->pitch / sizeof(uint32_t);
  SDL_DestroySurface(original_surface);
  return true;
}

framebuffer_3D *init_framebuffer_3D(SDL_Renderer *renderer, int view_width,
                                    int view_height, int view_start_x,
                                    int view_start_y, int draw_width,
                                    int draw_height) {
  framebuffer_3D *buffer = calloc(1, sizeof(framebuffer_3D));
  asset_pointers *ap = calloc(1, sizeof(asset_pointers));
  buffer->asset_pointers = ap;
  buffer->renderer = renderer;
  buffer->draw.width = draw_width;
  buffer->draw.height = draw_height;
  buffer->view_width = view_width;
  buffer->view_height = view_height;
  buffer->view_start_x = view_start_x;
  buffer->view_start_y = view_start_y;

  if (!init_draw_buffer(buffer)) {
    SDL_Log("Frame Buffer initilization failed, could not set up Pixel buffer");
    return NULL;
  }
  if (!init_raycast_3D_texture(buffer)) {
    SDL_Log("Frame Buffer initilization failed, could not set up the texture");
    return NULL;
  }
  if (!init_wall_buffer(buffer)) {
    SDL_Log("Frame Buffer initilization failed, could not load wall asset");
    return NULL;
  }
  if (!init_floor_buffer(buffer)) {
    SDL_Log("Frame buffer initilization failed, could not load floor asset");
    return false;
  }
  if (!init_ceiling_buffer(buffer)) {
    SDL_Log("Frame buffer initilization failed, could not load ceiling asset");
    return false;
  }
  return buffer;
}

bool render_framebuffer_3D(framebuffer_3D *buffer) {
  if (!SDL_UpdateTexture(buffer->raycast_3D_texture, NULL, buffer->draw.pixels,
                         buffer->draw.width * sizeof(uint32_t))) {
    SDL_Log("Failed to update framebuffer_3D texture: %s", SDL_GetError());
    return true;
  }

  SDL_FRect dest_rect = {buffer->view_start_x, buffer->view_start_y,
                         buffer->view_width, buffer->view_height};

  // SDL_FRect dest_rect = {800, 0, buffer->draw.width, buffer->draw.height};

  if (!SDL_RenderTexture(buffer->renderer, buffer->raycast_3D_texture, NULL,
                         &dest_rect)) {
    SDL_Log("Failed to Render texture to GPU: %s", SDL_GetError());
    return false;
  }
  int buffer_size = buffer->draw.width * buffer->draw.height * sizeof(uint32_t);
  memset(buffer->draw.pixels, 0, buffer_size);
  return true;
}

void destroy_framebuffer_3D(framebuffer_3D *buffer) {
  SDL_DestroyTexture(buffer->raycast_3D_texture);
  SDL_DestroySurface(buffer->asset_pointers->wall_surface);
  SDL_DestroySurface(buffer->asset_pointers->floor_surface);
  SDL_DestroySurface(buffer->asset_pointers->ceiling_surface);
  free(buffer->asset_pointers);
  free(buffer->draw.pixels);
  free(buffer);
  SDL_Log("3D Framebuffer has been destroyed. RIP");
}
