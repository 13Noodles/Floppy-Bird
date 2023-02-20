#ifndef FLOPPY_BIRD_SETTINGS_H
#define FLOPPY_BIRD_SETTINGS_H

#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "settingsValues.h"
#include "entities.h"

struct GameSettings {
  const Uint32 FPS;
  const float GRAVITY_STRENGTH;
  const float JUMP_STRENGTH;
  const int OBSTACLE_PASSAGE_MIN_HEIGHT;
  const int OBSTACLE_PASSAGE_MAX_HEIGHT;
  const int OBSTACLE_WIDTH;
  int obstacle_speed;
  const SDL_Rect PLAY_AREA;
};

struct DisplaySettings {
  const SDL_Color background_color;
  const SDL_Color player_color;
  const SDL_Color player_border_color;
  const SDL_Color obstacle_color;
  const SDL_Color obstacle_border_color;
  int window_width;
  int window_height;
};

struct GameObjects {
  struct Player player;
  struct Obstacle obstacles[MAX_OBSTACLES];
};

struct Spritesheet {
  SDL_Texture *texture;
  SDL_Rect sprite_size;
  int columns;
  int rows;
};

struct GameTextures {
  struct Spritesheet player_spritesheet;
  struct Spritesheet obstacle_spritesheet;
  struct Spritesheet background_spritesheet;
};

struct GameTexturesPaths {
  const char* player_spritesheet_path;
  const char* obstacle_spritesheet_path;
  const char* background_spritesheet_path;
};

struct TexturesSettings{ // will change to implement spritesheets
  struct GameTextures textures;
  struct GameTexturesPaths textures_paths;
};

#endif // FLOPPY_BIRD_SETTINGS_H
