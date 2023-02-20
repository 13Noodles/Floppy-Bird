#ifndef FLOPPY_BIRD_SETTINGS_VALUES_H
#define FLOPPY_BIRD_SETTINGS_VALUES_H

#define FPS_MAX 60

#define WINDOW_USE_FIXED_SIZE false
#define WINDOW_BORDERLESS false

// used if (WINDOW_USE_FIXED_SIZE == false)
#define WINDOW_WIDTH_DISPLAY_RATIO 0.95
#define WINDOW_HEIGHT_DISPLAY_RATIO 0.75

// used if (WINDOW_USE_FIXED_SIZE == true)
#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 480


#define MAX_OBSTACLES 10

// spritesheets info
#define PLAYER_SPRITESHEET_COLUMNS 3
#define PLAYER_SPRITESHEET_ROWS 3
#define OBSTACLE_SPRITESHEET_COLUMNS 2
#define OBSTACLE_SPRITESHEET_ROWS 1
#define BACKGROUND_SPRITESHEET_COLUMNS 2
#define BACKGROUND_SPRITESHEET_ROWS 1

enum PlayerSpriteId {
  PLAYER_BLUE,
  PLAYER_RED,
  PLAYER_YELLOW
};
enum ObstacleSpriteId {
  OBSTACLE_GREEN,
  OBSTACLE_RED
};
enum BackgroundSpriteId {
  BACKGROUND_DAY,
  BACKGROUND_NIGHT
};

#endif // FLOPPY_BIRD_SETTINGS_VALUES_H
