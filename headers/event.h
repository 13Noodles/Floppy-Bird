#ifndef FLOPPY_BIRD_EVENT_H
#define FLOPPY_BIRD_EVENT_H

#include <stdbool.h>
#include "SDL_video.h"
#include "game.h"
#include "engine.h"

void process_events(struct Game *game);

void process_key_event(struct Game *game, SDL_Keycode key, bool pressed);

#endif //FLOPPY_BIRD_EVENT_H
