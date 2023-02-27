#include "message.h"

int initialize_message(struct Message *message_ptr, TTF_Font *font_ptr,
                       const char *text, SDL_Color color,
                       enum MessageAlignement alignement, SDL_FRect box_rect,
                       bool to_be_shown) {
  message_ptr->font = font_ptr;
  message_ptr->box_rect = box_rect;
  message_ptr->alignement = alignement;
  strncpy(message_ptr->text, text, MESSAGE_TEXT_LENGTH - 1);
  message_ptr->color = color;
  message_ptr->is_to_be_drawn = to_be_shown;
  update_message_rect(message_ptr,box_rect);
  return 0;
}

void update_message_text(struct Message *message, const char *text) {
  if (strlen(message->text) == 0) {
    char text[] = "uh, forgot message's text?";
    strncpy(message->text, text, MESSAGE_TEXT_LENGTH);
  }else {
    strncpy(message->text, text, MESSAGE_TEXT_LENGTH - 1);
  }
  update_message_rect(message,message->box_rect);
}

void update_message_rect(struct Message *message, SDL_FRect rect) {
  int texture_width, texture_height;
  message->box_rect = rect;
  SDL_QueryTexture(message->texture, NULL, NULL, &texture_width,
                   &texture_height);
  float ratio = 1.0f;
  if (message->fit == FIT_BOX) {
    ratio = SDL_min(message->box_rect.w / texture_width,
                    message->box_rect.h / texture_height);
  } else if (message->fit == FIT_BOX_HORIZONTAL) {
    ratio = message->box_rect.w / texture_width;
  } else if (message->fit == FIT_BOX_VERTICAL) {
    ratio = message->box_rect.h / texture_height;
  }

  message->draw_rect.w = (float)texture_width * ratio;
  message->draw_rect.h = (float)texture_height * ratio;

  // process x axis
  if (message->alignement == ALIGN_TOP_MIDDLE ||
      message->alignement == ALIGN_CENTER ||
      message->alignement == ALIGN_BOTTOM_MIDDLE) {
    rect.x += (message->box_rect.w - message->draw_rect.w) / 2.0f;
  } else if (message->alignement == ALIGN_TOP_RIGHT ||
             message->alignement == ALIGN_MIDDLE_RIGHT ||
             message->alignement == ALIGN_BOTTOM_RIGHT) {
    rect.x += (message->box_rect.w - message->draw_rect.w);
  }
  // process y axis
  if (message->alignement == ALIGN_MIDDLE_LEFT ||
      message->alignement == ALIGN_CENTER ||
      message->alignement == ALIGN_MIDDLE_RIGHT) {
    rect.y += (message->box_rect.h - message->draw_rect.h) / 2.0f;
  } else if (message->alignement == ALIGN_BOTTOM_LEFT ||
             message->alignement == ALIGN_BOTTOM_MIDDLE ||
             message->alignement == ALIGN_BOTTOM_RIGHT) {
    rect.y += (message->box_rect.h - message->draw_rect.h);
  }
  message->draw_rect.x = rect.x;
  message->draw_rect.y = rect.y;
  message->texture_needs_update = true;
}

void update_message_texture(SDL_Renderer *renderer, struct Message *message) {
  SDL_Surface *temp_surface =
      TTF_RenderUTF8_Solid(message->font, message->text, message->color);
  message->texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
  SDL_FreeSurface(temp_surface);
  update_message_rect(message, message->box_rect);
  message->texture_needs_update = false;
}
