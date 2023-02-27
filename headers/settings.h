#ifndef FLOPPY_BIRD_SETTINGS_H
#define FLOPPY_BIRD_SETTINGS_H

#include "SDL_pixels.h"
#include "SDL_render.h"
#include "SDL_stdinc.h"
#include "SDL_ttf.h"
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
  unsigned int columns;
  unsigned int rows;
};

struct Message {
  SDL_FRect draw_rect;
  SDL_FRect box_rect;
  SDL_Texture *texture;
  TTF_Font *font;
  SDL_Color color;
  enum MessageAlignement alignement;
  enum MessageFitBox fit;
  bool is_to_be_drawn;
  bool texture_needs_update;
  char text[MESSAGE_TEXT_LENGTH];
};

struct GameSpritesheets {
  struct Spritesheet player_spritesheet;
  struct Spritesheet obstacle_spritesheet;
  struct Spritesheet background_spritesheet;
};

struct GameMessages {
  struct Message score_message;
  struct Message game_over_message;
  struct Message pause_message;
  struct Message *messages_array[3];
  size_t message_count;
};

struct GameFonts {
  TTF_Font *default_font;
  TTF_Font *debug_font;
};

struct TexturesPaths {
  const char* player_spritesheet_path;
  const char* obstacle_spritesheet_path;
  const char* background_spritesheet_path;
};

struct FontsPaths {
  const char* default_font_path;
  const char* debug_font_path;
};

struct GamePathSettings{
  struct FontsPaths fonts_paths;
  struct TexturesPaths textures_paths;
};


#endif // FLOPPY_BIRD_SETTINGS_H
