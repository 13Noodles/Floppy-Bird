#include "draw.h"

void draw(SDL_Renderer *renderer, struct GameObjects objects,
          struct GameSpritesheets game_spritesheets,
          struct GameMessages *game_messages,
          struct DisplaySettings display_settings) {
  update_messages_textures(renderer,game_messages);
  set_color(renderer, display_settings.background_color);
  SDL_RenderClear(renderer);
  draw_background(renderer, game_spritesheets.background_spritesheet);
  draw_player(renderer, objects.player, game_spritesheets.player_spritesheet);
  draw_obstacles(renderer, objects.obstacles,
                 game_spritesheets.obstacle_spritesheet);
  draw_messages(renderer,*game_messages);
  SDL_RenderPresent(renderer);
}

void draw_background(SDL_Renderer *renderer,
                     struct Spritesheet background_spritesheet) {
  SDL_Rect src_rect;
  rect_from_sprite_id(background_spritesheet, BACKGROUND_DAY, &src_rect);
  SDL_RenderCopy(renderer, background_spritesheet.texture, &src_rect, NULL);
}

void draw_player(SDL_Renderer *renderer, struct Player player,
                 struct Spritesheet player_spritesheet) {
  SDL_Rect src_rect = player_spritesheet.sprite_size;
  int sprite_id = player.sprite_id;
  if (player.velocity_y == 0) {
    sprite_id += 1;
  } else if (player.velocity_y > 0) { // player.velocity_y == 0
    sprite_id += 2;
  }
  rect_from_sprite_id(player_spritesheet, sprite_id, &src_rect);
  SDL_RenderCopy(renderer, player_spritesheet.texture, &src_rect, &player.rect);
}

void draw_obstacles(SDL_Renderer *renderer,
                    struct Obstacle obstacles[MAX_OBSTACLES],
                    struct Spritesheet obstacle_spritesheet) {
  SDL_Rect src_rect;
  for (size_t i = 0; i < MAX_OBSTACLES; ++i) {
    if (obstacles[i].is_used) {
      rect_from_sprite_id(obstacle_spritesheet, obstacles[i].sprite_id,
                          &src_rect);
      SDL_RenderCopy(renderer, obstacle_spritesheet.texture, &src_rect,
                     &obstacles[i].bottom_rect);
      SDL_RenderCopyEx(renderer, obstacle_spritesheet.texture, &src_rect,
                       &obstacles[i].top_rect, 180, NULL, SDL_FLIP_NONE);
    }
  }
}

void draw_messages(SDL_Renderer *renderer, struct GameMessages messages){
  for (size_t i = 0; i < messages.message_count; ++i) {
    if(messages.messages_array[i]->is_to_be_drawn){
      SDL_RenderCopyF(renderer,messages.messages_array[i]->texture,NULL,&messages.messages_array[i]->draw_rect);
    }
  }
}
void update_messages_textures(SDL_Renderer *renderer, struct GameMessages *messages){
  for(size_t i = 0;i < messages->message_count; ++i){
    if(messages->messages_array[i]->texture_needs_update == true){
      update_message_texture(renderer,messages->messages_array[i]);
    }
  }
}

int rect_from_sprite_id(struct Spritesheet spritesheet, unsigned int sprite_id,
                        SDL_Rect *rect) {
  unsigned int column = sprite_id % spritesheet.columns;
  unsigned int row = (sprite_id - column) / spritesheet.columns;
  if (row >= spritesheet.rows) {
    printf("rect_from_sprite_id error : sprite_id=%d (expected a value between "
           "0 and %d)\n",
           sprite_id, spritesheet.columns * spritesheet.rows);
    return -1;
  }
  rect->x = column * spritesheet.sprite_size.w;
  rect->y = row * spritesheet.sprite_size.h;
  rect->w = spritesheet.sprite_size.w;
  rect->h = spritesheet.sprite_size.h;
  return 0;
}

