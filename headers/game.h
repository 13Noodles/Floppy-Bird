#ifndef FLOPPY_BIRD_GAME_H
#define FLOPPY_BIRD_GAME_H

#include "settings.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>

typedef enum GameState {
  STATE_PLAYING,
  STATE_LOST,
  STATE_PAUSED,
  STATE_QUIT
} game_state_t;

typedef struct Game {
  enum GameState state;
  struct GameSettings game_settings;
  struct DisplaySettings display_settings;
  struct GameObjects game_objects;
  struct TexturesSettings texture_settings;
  int score;
} game_t;

int initialize_texture(SDL_Renderer *renderer, const char* path, SDL_Texture **texture);
int initialize_game_textures(SDL_Renderer *renderer,struct GameTextures *game_textures, struct GameTexturesPaths texture_paths);
int initialize_game(game_t *game, SDL_Renderer *renderer);

void game_lost(struct Game *game);
void game_restart(struct Game *game);
void game_pause(struct Game *game);
void game_resume(struct Game *game);

void destroy_game_textures(struct Game *game);
void stop_game(struct Game *game);

void increase_score(struct Game *game);
#endif //FLOPPY_BIRD_GAME_H
