#ifndef FLOPPY_BIRD_EVENT_H
#define FLOPPY_BIRD_EVENT_H

#include <stdbool.h>
#include "SDL_video.h"
#include "engine.h"
#include "game.h"

void process_events(struct Game *game);

void process_key_event(struct Game *game, SDL_Keycode key, bool pressed);

#endif // FLOPPY_BIRD_EVENT_H
