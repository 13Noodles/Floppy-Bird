#include "game.h"
#include "engine.h"

int initialize_game(struct Game *game,SDL_Renderer *renderer){ 
  if(initialize_game_textures(renderer,&game->texture_settings.textures,game->texture_settings.textures_paths) == -1){
    printf("Game texture initialization failed : %s\n",IMG_GetError());
    return -1;
  }
  game->score = 0;
  for (size_t i = 0; i < MAX_OBSTACLES; ++i) {
    game->game_objects.obstacles[i].is_used = false;
  }
  add_obstacle(game->game_objects.obstacles,game->game_settings);
  return 0;
}

int initialize_game_textures(SDL_Renderer *renderer, struct GameTextures *game_textures, struct GameTexturesPaths texture_paths){
  int width,height;
  if(initialize_texture(renderer,texture_paths.player_spritesheet_path,&game_textures->player_spritesheet.texture) == -1){
    return -1;
  }
  SDL_QueryTexture(game_textures->player_spritesheet.texture,NULL,NULL,&width,&height);
  game_textures->player_spritesheet.sprite_size.x = 0;
  game_textures->player_spritesheet.sprite_size.y = 0;
  game_textures->player_spritesheet.sprite_size.w = width/PLAYER_SPRITESHEET_COLUMNS;
  game_textures->player_spritesheet.sprite_size.h = height/PLAYER_SPRITESHEET_ROWS;
  game_textures->player_spritesheet.columns = PLAYER_SPRITESHEET_COLUMNS;
  game_textures->player_spritesheet.rows = PLAYER_SPRITESHEET_ROWS;

if(initialize_texture(renderer,texture_paths.obstacle_spritesheet_path,&game_textures->obstacle_spritesheet.texture) == -1){
    return -1;
  }
  SDL_QueryTexture(game_textures->obstacle_spritesheet.texture,NULL,NULL,&width,&height);
  game_textures->obstacle_spritesheet.sprite_size.x = 0;
  game_textures->obstacle_spritesheet.sprite_size.y = 0;
  game_textures->obstacle_spritesheet.sprite_size.w = width/OBSTACLE_SPRITESHEET_COLUMNS;
  game_textures->obstacle_spritesheet.sprite_size.h = height/OBSTACLE_SPRITESHEET_ROWS;
  game_textures->obstacle_spritesheet.columns = OBSTACLE_SPRITESHEET_COLUMNS;
  game_textures->obstacle_spritesheet.rows = OBSTACLE_SPRITESHEET_ROWS;
  
  if(initialize_texture(renderer,texture_paths.background_spritesheet_path,&game_textures->background_spritesheet.texture) == -1){
    return -1;
  }
  SDL_QueryTexture(game_textures->background_spritesheet.texture,NULL,NULL,&width,&height);
  game_textures->background_spritesheet.sprite_size.x = 0;
  game_textures->background_spritesheet.sprite_size.y = 0;
  game_textures->background_spritesheet.sprite_size.w = width/BACKGROUND_SPRITESHEET_COLUMNS;
  game_textures->background_spritesheet.sprite_size.h = height/BACKGROUND_SPRITESHEET_ROWS;
  game_textures->background_spritesheet.columns = BACKGROUND_SPRITESHEET_COLUMNS;
  game_textures->background_spritesheet.rows = BACKGROUND_SPRITESHEET_ROWS;
  
  return 0;
}

int initialize_texture(SDL_Renderer *renderer, const char* path, SDL_Texture **dest){
  *dest = IMG_LoadTexture(renderer,path);
  if(*dest == NULL){
    return -1;
  }
  return 0;
}

void game_lost(struct Game *game){
  if(game->game_objects.player.velocity_y<0){
    game->game_objects.player.velocity_y = 0;
  }
  game->game_objects.player.is_diving = true;
  game->state = STATE_LOST;
}

void game_restart(struct Game *game){
  if(game->state != STATE_PLAYING){
    for (size_t i = 0; i < MAX_OBSTACLES; ++i) {
      game->game_objects.obstacles[i].is_used = false;
    }
    add_obstacle(game->game_objects.obstacles,game->game_settings);
    game->score = 0;
    game->game_objects.player.rect.y = game->game_settings.PLAY_AREA.y+game->game_settings.PLAY_AREA.h/2-game->game_objects.player.rect.h/2;
    game->game_objects.player.velocity_y = 0;
    game->game_objects.player.is_diving = false;
    game->state = STATE_PLAYING;
    printf("Game restart~");
  }
}

void game_pause(struct Game *game){
  if(game->state == STATE_PLAYING){
    game->state = STATE_PAUSED;
    printf("Game paused\n");
  }
}

void game_resume(struct Game *game){
  if(game->state == STATE_PAUSED){
    game->state = STATE_PLAYING;
    printf("Game resumed\n");
  }
}

void stop_game(struct Game *game){
  destroy_game_textures(game);
  game->state = STATE_QUIT;
  printf("Game stop called~\n");
}

void destroy_game_textures(struct Game *game){
  SDL_DestroyTexture(game->texture_settings.textures.player_spritesheet.texture);
  SDL_DestroyTexture(game->texture_settings.textures.obstacle_spritesheet.texture);
  SDL_DestroyTexture(game->texture_settings.textures.background_spritesheet.texture);
}

void increase_score(game_t *game){
  game->score += 1;
  //game->game_settings.obstacle_speed *= 1.05f;
  printf("Score increased, current score : %d\n",game->score);
  //printf("\tobstacle speed : %f\n",game->game_settings.obstacle_speed);
}
