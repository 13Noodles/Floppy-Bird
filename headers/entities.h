#ifndef FLOPPY_BIRD_OBJECTS
#define FLOPPY_BIRD_OBJECTS

#include <stdbool.h>
#include "SDL_rect.h"
#include "settingsValues.h"

struct Player {
  SDL_Rect rect;
  enum PlayerSpriteId sprite_id;
  float velocity_y;
  bool is_diving;
};

struct Obstacle {
  SDL_Rect top_rect;
  SDL_Rect bottom_rect;
  enum ObstacleSpriteId sprite_id;
  bool is_used;
};

#endif // FLOPPY_BIRD_OBJECTS
