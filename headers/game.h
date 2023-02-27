#ifndef FLOPPY_BIRD_GAME_H
#define FLOPPY_BIRD_GAME_H

#include "SDL_image.h"
#include "message.h"
#include "settings.h"
#include <stdbool.h>
#include <stdio.h>

typedef enum GameState {
  STATE_PLAYING,
  STATE_LOST,
  STATE_PAUSED,
  STATE_QUIT
} game_state_t;

struct Game {
  enum GameState state;
  struct GameSettings game_settings;
  struct DisplaySettings display_settings;
  struct GameObjects game_objects;
  struct GameSpritesheets spritesheets;
  struct GameFonts fonts;
  struct GameMessages messages;
  struct GamePathSettings paths_settings;
  unsigned int score;
};

int initialize_spritesheet(SDL_Renderer *renderer,
                           struct Spritesheet *spritesheet_ptr,
                           const char *spritesheet_file_path,
                           unsigned int column_count, unsigned int row_count);

int initialize_game_spritesheets(SDL_Renderer *renderer,
                                 struct GameSpritesheets *spritesheets_ptr,
                                 struct TexturesPaths paths);

int initialize_game_fonts(struct GameFonts *game_fonts,
                          struct FontsPaths paths);
int initialize_game_messages(struct GameMessages *messages_ptr,
                             struct GameFonts *fonts);

int initialize_game(struct Game *game, SDL_Renderer *renderer);

void destroy_game_textures(struct GameSpritesheets *spritesheets);
void destroy_game_fonts(struct GameFonts *fonts);
void destroy_game(struct Game *game);

void game_lost(struct Game *game);
void game_restart(struct Game *game);
void game_pause(struct Game *game);
void game_resume(struct Game *game);
void game_stop(struct Game *game);

void increase_score(struct Game *game);

void update_score_message(struct Game *game);
int add_obstacle(struct Obstacle obstacles[MAX_OBSTACLES],
                 struct GameSettings game_settings);

#endif // FLOPPY_BIRD_GAME_H
