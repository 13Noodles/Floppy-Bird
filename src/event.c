#include "event.h"

void process_events(struct Game *game) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      game_stop(game);
    } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
      process_key_event(game, event.key.keysym.sym,
                        event.key.state == SDL_PRESSED);
    } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_FOCUS_LOST){
      if(game->state == STATE_PLAYING){
        game_pause(game);
      }
    }
  }
}

void process_key_event(struct Game *game, SDL_Keycode key, bool pressed) {
  if (key == SDLK_ESCAPE) {
    game_stop(game);
    return;
  }
  if (pressed) {
    if (game->state == STATE_LOST && key == SDLK_r) {
      game_restart(game);
    } else if (game->state == STATE_PAUSED) {
      if(key == SDLK_LSHIFT){
        game_resume(game);
      }
    } else if(game->state == STATE_PLAYING){
      if(key == SDLK_LSHIFT || key == SDLK_RSHIFT){
        game_pause(game);
      }else if (key == SDLK_SPACE || key == SDLK_z || key == SDLK_UP) {
        jump(&game->game_objects.player, game->game_settings.JUMP_STRENGTH);
      }else if(key == SDLK_LALT || key == SDLK_RALT || key == SDLK_s || key == SDLK_DOWN){
        start_diving(&game->game_objects.player);
      } else if (key == SDLK_x) {
        add_obstacle(game->game_objects.obstacles, game->game_settings);
      } else {
        printf("unhandled key : %s (pressed:%d)\n", SDL_GetKeyName(key),
               pressed);
      }
    }
  }else{ // pressed == false ie. key released
    if(key == SDLK_LALT || key == SDLK_RALT || key == SDLK_s || key == SDLK_DOWN){
      stop_diving(&game->game_objects.player);
    }
  }
}
