#ifndef FLOPPY_BIRD_MESSAGE_H
#define FLOPPY_BIRD_MESSAGE_H

#include "SDL_rect.h"
#include "SDL_render.h"
#include "settings.h"
#include "settingsValues.h"

int initialize_message(struct Message *message_ptr, TTF_Font *font_ptr,
                       const char *text, SDL_Color color,
                       enum MessageAlignement alignement, SDL_FRect box_rect,
                       bool te_be_shown);

void update_message_text(struct Message *message, const char* text);
void update_message_texture(SDL_Renderer *renderer, struct Message *message);
void update_message_rect(struct Message *message, SDL_FRect rect);

#endif // FLOPPY_BIRD_MESSAGE_H
