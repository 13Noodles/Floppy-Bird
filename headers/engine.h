#ifndef FLOPPY_BIRD_UPDATE_H
#define FLOPPY_BIRD_UPDATE_H

#include <stdlib.h>
#include "SDL2/SDL_stdinc.h"
#include "entities.h"
#include "settings.h"
#include "game.h"

void update(struct Game *game,
            Uint32 delta_ms);
int update_player(struct Player *player, struct GameSettings game_settings,
                  Uint32 delta_ms);

int update_obstacles(struct Obstacle obstacles[MAX_OBSTACLES],
                     struct GameSettings game_settings, Uint32 delta_ms);

int check_collisions(struct Player player, struct Obstacle obstacles[MAX_OBSTACLES]);

int start_diving(struct Player *player);
int stop_diving(struct Player *player);
int jump(struct Player *player, int jump_strength);

#endif // FLOPPY_BIRD_UPDATE_H
