#include "game.h"

/*************************************/
/*                                   */
/*  Spritesheet & Message functions  */
/*                                   */
/*************************************/

int initialize_spritesheet(SDL_Renderer *renderer,
                           struct Spritesheet *spritesheet_ptr,
                           const char *spritesheet_file_path,
                           unsigned int column_count, unsigned int row_count) {
  if (column_count == 0 || row_count == 0) {
    printf("spritesheet initialization error : column_count or row_count equal "
           "to 0\n");
    return -1;
  }
  spritesheet_ptr->texture = IMG_LoadTexture(renderer, spritesheet_file_path);
  if (spritesheet_ptr->texture == NULL) {
    return -1;
  }
  int width;
  int height;
  SDL_QueryTexture(spritesheet_ptr->texture, NULL, NULL, &width, &height);
  spritesheet_ptr->sprite_size.x = 0;
  spritesheet_ptr->sprite_size.y = 0;
  spritesheet_ptr->sprite_size.w = width / column_count;
  spritesheet_ptr->sprite_size.h = height / row_count;
  spritesheet_ptr->columns = column_count;
  spritesheet_ptr->rows = row_count;
  return 0;
}

/***********************************/
/*                                 */
/*  Game initialization functions  */
/*                                 */
/***********************************/
int initialize_game_fonts(struct GameFonts *game_fonts,
                          struct FontsPaths paths) {
  game_fonts->default_font = TTF_OpenFont(paths.default_font_path, 20);
  if (game_fonts->default_font == NULL) {
    return -1;
  }
  game_fonts->debug_font = TTF_OpenFont(paths.debug_font_path, 20);
  if (game_fonts->debug_font == NULL) {
    return -1;
  }
  return 0;
}
int initialize_game_messages(struct GameMessages *messages_ptr,
                             struct GameFonts *fonts) {
  messages_ptr->score_message.font = fonts->default_font;
  messages_ptr->score_message.texture_needs_update = true;
  messages_ptr->score_message.is_to_be_drawn = true;

  messages_ptr->game_over_message.font = fonts->default_font;
  messages_ptr->game_over_message.texture_needs_update = true;

  messages_ptr->pause_message.font = fonts->default_font;
  messages_ptr->pause_message.texture_needs_update = true;
  // messages_ptr->game_over_message.is_to_be_drawn = false;
  // messages_ptr->pause_message.is_to_be_drawn = false
  
  messages_ptr->messages_array[0] = &messages_ptr->score_message;
  messages_ptr->messages_array[1] = &messages_ptr->game_over_message;
  messages_ptr->messages_array[2] = &messages_ptr->pause_message;
  messages_ptr->message_count = 3;
  return 0;
}

int initialize_game_spritesheets(SDL_Renderer *renderer,
                                 struct GameSpritesheets *spritesheets_ptr,
                                 struct TexturesPaths paths) {
  if (initialize_spritesheet(renderer, &spritesheets_ptr->player_spritesheet,
                             paths.player_spritesheet_path,
                             PLAYER_SPRITESHEET_COLUMNS,
                             PLAYER_SPRITESHEET_ROWS) == -1) {
    return -1;
  }
  if (initialize_spritesheet(renderer, &spritesheets_ptr->obstacle_spritesheet,
                             paths.obstacle_spritesheet_path,
                             OBSTACLE_SPRITESHEET_COLUMNS,
                             OBSTACLE_SPRITESHEET_ROWS) == -1) {
    return -1;
  }
  if (initialize_spritesheet(
          renderer, &spritesheets_ptr->background_spritesheet,
          paths.background_spritesheet_path, BACKGROUND_SPRITESHEET_COLUMNS,
          BACKGROUND_SPRITESHEET_ROWS) == -1) {
    return -1;
  }
  return 0;
}

int initialize_game(struct Game *game, SDL_Renderer *renderer) {
  if (initialize_game_spritesheets(renderer, &game->spritesheets,
                                   game->paths_settings.textures_paths) == -1) {
    printf("Game sprites initialization failed : %s\n", IMG_GetError());
    return -1;
  }
  if (initialize_game_fonts(&game->fonts, game->paths_settings.fonts_paths) ==
      -1) {
    printf("Game fonts initialization failed : %s\n", TTF_GetError());
    return -1;
  }
  if (initialize_game_messages(&game->messages, &game->fonts) == -1) {
    printf("Game messages initialization failed : %s\n", IMG_GetError());
    return -1;
  }
  game->score = 0;
  update_score_message(game);
  for (size_t i = 0; i < MAX_OBSTACLES; ++i) {
    game->game_objects.obstacles[i].is_used = false;
  }
  add_obstacle(game->game_objects.obstacles, game->game_settings);
  return 0;
}

/*************************************/
/*                                   */
/*  Game deinitialization / destroy  */
/*                                   */
/*************************************/
void destroy_game(struct Game *game) {
  destroy_game_textures(&game->spritesheets);
  destroy_game_fonts(&game->fonts);
  game->state = STATE_QUIT;
}

void destroy_game_fonts(struct GameFonts *game_fonts) {
  TTF_CloseFont(game_fonts->default_font);
  TTF_CloseFont(game_fonts->debug_font);
  return;
}

void destroy_game_textures(struct GameSpritesheets *game_spritesheets) {
  SDL_DestroyTexture(game_spritesheets->player_spritesheet.texture);
  SDL_DestroyTexture(game_spritesheets->obstacle_spritesheet.texture);
  SDL_DestroyTexture(game_spritesheets->background_spritesheet.texture);
}

/*****************/
/*               */
/*  Game states  */
/*               */
/*****************/

void game_restart(struct Game *game) {
  if (game->state != STATE_PLAYING) {
    for (size_t i = 0; i < MAX_OBSTACLES; ++i) {
      game->game_objects.obstacles[i].is_used = false;
    }
    add_obstacle(game->game_objects.obstacles, game->game_settings);
    game->score = 0;
    update_score_message(game);
    game->game_objects.player.rect.y = game->game_settings.PLAY_AREA.y +
                                       game->game_settings.PLAY_AREA.h / 2 -
                                       game->game_objects.player.rect.h / 2;
    game->game_objects.player.velocity_y = 0;
    game->game_objects.player.is_diving = false;
    game->state = STATE_PLAYING;
    game->messages.game_over_message.is_to_be_drawn = false;
    printf("Game restart~");
  }
}
void game_lost(struct Game *game) {
  if (game->game_objects.player.velocity_y < 0) {
    game->game_objects.player.velocity_y = 0;
  }
  game->game_objects.player.is_diving = true;
  game->state = STATE_LOST;
  game->messages.game_over_message.is_to_be_drawn = true;
}

void game_pause(struct Game *game) {
  if (game->state == STATE_PLAYING) {
    game->state = STATE_PAUSED;
    game->messages.pause_message.is_to_be_drawn = true;
  }
}

void game_resume(struct Game *game) {
  if (game->state == STATE_PAUSED) {
    game->state = STATE_PLAYING;
    game->messages.pause_message.is_to_be_drawn = false;
  }
}

void game_stop(struct Game *game) {
  game->state = STATE_QUIT;
  printf("Game quit called~\n");
}

void increase_score(struct Game *game) {
  game->score += 1;
  // game->game_settings.obstacle_speed *= 1.05f;
  update_score_message(game);
}

void update_score_message(struct Game *game) {
  sprintf(game->messages.score_message.text, "Score : %d", game->score);
  game->messages.score_message.texture_needs_update = true;
}

/*
*
* can't move add_obstacle into engine.h/engine.c because it
* causes a recrusive include..
* might want to move all the objects into engine and us
* game.h/game.c only to handle states/messages etc
* and hold the engine IN game's struct not alongside
*
*/
int add_obstacle(struct Obstacle obstacles[MAX_OBSTACLES],
                 struct GameSettings game_settings) {
  int obstacle_id = 0;
  int passage_height = game_settings.OBSTACLE_PASSAGE_MIN_HEIGHT +
                       rand() % (game_settings.OBSTACLE_PASSAGE_MAX_HEIGHT -
                                 game_settings.OBSTACLE_PASSAGE_MIN_HEIGHT);
  int top_obstacle_height =
      rand() % (game_settings.PLAY_AREA.h - passage_height);

  obstacles[obstacle_id].top_rect.x =
      game_settings.PLAY_AREA.x + game_settings.PLAY_AREA.w;
  obstacles[obstacle_id].top_rect.y = game_settings.PLAY_AREA.y;
  obstacles[obstacle_id].top_rect.w = game_settings.OBSTACLE_WIDTH;
  obstacles[obstacle_id].top_rect.h = top_obstacle_height;

  obstacles[obstacle_id].bottom_rect.x =
      game_settings.PLAY_AREA.x + game_settings.PLAY_AREA.w;
  obstacles[obstacle_id].bottom_rect.y = top_obstacle_height + passage_height;
  obstacles[obstacle_id].bottom_rect.w = game_settings.OBSTACLE_WIDTH;
  obstacles[obstacle_id].bottom_rect.h =
      game_settings.PLAY_AREA.h - obstacles[obstacle_id].bottom_rect.y;
  obstacles[obstacle_id].is_used = true;
  obstacles[obstacle_id].sprite_id = rand() % 2;
  return 0;
}
