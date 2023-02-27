#ifndef FLOPPY_BIRD_DRAW_H
#define FLOPPY_BIRD_DRAW_H

#include "SDL2/SDL_render.h"
#include "entities.h"
#include "message.h"
#include "settings.h"

void draw(SDL_Renderer *renderer, struct GameObjects objects,
          struct GameSpritesheets game_spritesheets,
          struct GameMessages *game_messages,
          struct DisplaySettings display_settings);

void draw_background(SDL_Renderer *renderer,
                     struct Spritesheet background_spritesheet);

void draw_player(SDL_Renderer *renderer, struct Player,
                 struct Spritesheet player_spritesheet);

void draw_obstacles(SDL_Renderer *renderer,
                    struct Obstacle obstacles[MAX_OBSTACLES],
                    struct Spritesheet obstacle_spritesheet);

void draw_messages(SDL_Renderer *renderer, struct GameMessages messages);


int rect_from_sprite_id(struct Spritesheet spritesheet, unsigned int sprite_id,
                        SDL_Rect *rect);

void update_messages_textures(SDL_Renderer *renderer, struct GameMessages *messages);
static inline void set_color(SDL_Renderer *renderer, SDL_Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

#endif // FLOPPY_BIRD_DRAW_H
