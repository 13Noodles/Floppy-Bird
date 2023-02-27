#include "engine.h"

void update(struct Game *game, Uint32 delta_ms) {
  // game->state == STATE_PLAYING or STATE_LOST
  if (game->state == STATE_PLAYING) {
    update_player(&game->game_objects.player, game->game_settings, delta_ms);
  int removed_obstacles = update_obstacles(game->game_objects.obstacles,
                                               game->game_settings, delta_ms);
      if (removed_obstacles) {
        add_obstacle(game->game_objects.obstacles, game->game_settings);
        increase_score(game);
      }
      if (check_collisions(game->game_objects.player,
                           game->game_objects.obstacles) != 0) {
        game_lost(game);
      }
  }else if (game->state == STATE_LOST) { 
    update_player(&game->game_objects.player, game->game_settings, delta_ms);
    //if(game->game_objects.player.rect.y+game->game_objects.player.rect.h<game->game_settings.PLAY_AREA.y+game->game_settings.PLAY_AREA.h){
    //  update_obstacles(game->game_objects.obstacles,game->game_settings,delta_ms);
    //}
  }
}

int update_player(struct Player *player, struct GameSettings game_settings,
                  Uint32 delta_ms) {
  int delta_v = game_settings.GRAVITY_STRENGTH * delta_ms / 100;
  if(player->is_diving){
    player->velocity_y += 3*delta_v;
  }else{
    player->velocity_y += delta_v;
  }
  player->rect.y += player->velocity_y * delta_ms / 100;

  if (player->rect.y + player->rect.h >=
      game_settings.PLAY_AREA.y + game_settings.PLAY_AREA.h) {
    player->velocity_y = 0;
    player->rect.y =
        game_settings.PLAY_AREA.y + game_settings.PLAY_AREA.h - player->rect.h;
    return -1;
  } else if (player->rect.y < game_settings.PLAY_AREA.y) {
    player->velocity_y = 0;
    player->rect.y = game_settings.PLAY_AREA.y;
    return 1;
  }
  return 0;
}

int update_obstacles(struct Obstacle obstacles[MAX_OBSTACLES],
                     struct GameSettings game_settings, Uint32 delta_ms) {
  int dx = game_settings.obstacle_speed * delta_ms / 100;
  int obstacles_out = 0;
  for (size_t i = 0; i < MAX_OBSTACLES; ++i) {
    if (obstacles[i].is_used) {
      obstacles[i].top_rect.x -= dx;
      obstacles[i].bottom_rect.x -= dx;
      if (obstacles[i].top_rect.x + obstacles[i].top_rect.w <
              game_settings.PLAY_AREA.x ||
          obstacles[i].bottom_rect.x + obstacles->bottom_rect.w <
              game_settings.PLAY_AREA.x) {
        obstacles_out += 1;
        obstacles[i].is_used = false;
      }
    }
  }
  return obstacles_out;
}

int check_collisions(struct Player player,
                     struct Obstacle obstacles[MAX_OBSTACLES]) {
  for (size_t i = 0; i < MAX_OBSTACLES; ++i) {
    if (SDL_HasIntersection(&player.rect, &obstacles[i].top_rect) == SDL_TRUE ||
        SDL_HasIntersection(&player.rect, &obstacles->bottom_rect) ==
            SDL_TRUE) {
      return 1;
    }
  }
  return 0;
}

int start_diving(struct Player *player){
  if(player->is_diving){
    return -1;
  }
  player->is_diving = true;
  if(player->velocity_y>0){
    player->velocity_y = 0;
  }
  return 0;
}
int stop_diving(struct Player *player){
  if(!player->is_diving){
    return -1;
  }
  player->is_diving = false;
  return 0;
}

int jump(struct Player *player, int jump_strength) {
  player->velocity_y = -jump_strength;
  player->sprite_id = (player->sprite_id+3*(1+rand()%2))%9;
  return 0;
}


