#include <stdbool.h>
#include <stdio.h>
#include <time.h> // used as seed for srand

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "settings.h"
#include "settingsValues.h"
#include "engine.h"
#include "draw.h"
#include "event.h"
#include "game.h"

int main() {
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("SDL init error : %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  if (IMG_Init(IMG_INIT_PNG) == 0) {
      printf("SDL_IMG init error : %s\n", IMG_GetError());
      return -1;
  }

  if (TTF_Init() < 0) {
      printf("SDL_TTF init error : %s\n", TTF_GetError());
      return -1;
  }

  int window_width, window_height;
  #if (WINDOW_USE_FIXED_SIZE == false)
    SDL_DisplayMode DISPLAY_MODE;
    SDL_GetCurrentDisplayMode(0, &DISPLAY_MODE);
    window_width = DISPLAY_MODE.w * WINDOW_WIDTH_DISPLAY_RATIO;
    window_height = DISPLAY_MODE.h * WINDOW_HEIGHT_DISPLAY_RATIO;
  #else 
    window_width = WINDOW_WIDTH;
    window_height = WINDOW_HEIGHT;
  #endif
  SDL_Window *window = SDL_CreateWindow(
      "13Noodles' Floppy Bird", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      window_width, window_height,
      // don't touch that change WINDOW_BORDERLESS in headers/settings.h instead
      WINDOW_BORDERLESS ? SDL_WINDOW_BORDERLESS : 0);
  if (window == NULL) {
    printf("SDL window init error : %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    printf("SDL renderer init error : %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  unsigned int seed = time(NULL);
  srand(seed);
  printf("Using [%d] as seed.\n", seed);
  int player_size = SDL_min(window_width/10,window_height/10);
  struct Game game = {
    .state = STATE_PLAYING, 
    .game_settings = {
      .FPS = FPS_MAX,
      .GRAVITY_STRENGTH = 2*window_height/100.0f,
      .JUMP_STRENGTH = window_height/20.0f,
      .PLAY_AREA = {0,0,window_width,window_height},
      .OBSTACLE_PASSAGE_MIN_HEIGHT = player_size*2,
      .OBSTACLE_PASSAGE_MAX_HEIGHT = window_height/3*2,
      .OBSTACLE_WIDTH = player_size*2/3,
      .obstacle_speed = window_width/40.0f
    },
    .display_settings = {
          .background_color = {20, 0, 20, SDL_ALPHA_OPAQUE},
          .player_color = {100, 0, 100, SDL_ALPHA_OPAQUE},
          .player_border_color = {200, 0, 0, SDL_ALPHA_OPAQUE},
          .obstacle_color = {0, 100, 0, SDL_ALPHA_OPAQUE},
          .obstacle_border_color = {200, 0, 0, SDL_ALPHA_OPAQUE},
          .window_width = window_width,
          .window_height = window_height
    },
    .spritesheets = {
      .player_spritesheet = {},
      .obstacle_spritesheet = {},
      .background_spritesheet = {} 
    },
    .messages = {
      .score_message = {
        .box_rect = {0,0,window_width,window_height/20.0f},
        .color = {255,255,255,SDL_ALPHA_OPAQUE},
        .alignement = ALIGN_MIDDLE_RIGHT,
        .fit = FIT_BOX,
        .is_to_be_drawn = false // just in case I forget to initialize //DEBUG REMOVE ME
      },
      .game_over_message = {
        .box_rect = {window_width/6.0f,window_height/6.0f,window_width*2/3.0f,window_height*2/3.0f},
        .color = {255,55,55,SDL_ALPHA_OPAQUE},
        .alignement = ALIGN_CENTER,
        .fit = FIT_BOX,
        .text = "Game over, press [R] to replay",
        .is_to_be_drawn = false
      },
      .pause_message = {
        .box_rect = {window_width/6.0f,0,window_width*2/3.0f,window_height*9/10.0f},
        .color = {255,255,255,SDL_ALPHA_OPAQUE},
        .alignement = ALIGN_BOTTOM_MIDDLE,
        .fit = FIT_BOX,
        .text = "Game paused, press [LSHIFT] or [RSHIFT] to resume",
        .is_to_be_drawn = false
      }

    },
    .paths_settings = {
      .fonts_paths = {
        .default_font_path = "assets/PressStart2P.ttf",
        .debug_font_path = "assets/PressStart2P.ttf"
      },
      .textures_paths = {
        .player_spritesheet_path = "assets/birds.png",
        .obstacle_spritesheet_path = "assets/pipes.png",
        .background_spritesheet_path = "assets/backgrounds.png"
      }
    },
    .game_objects = {
      {
        .velocity_y = 0.0f,
        .rect = {window_width/4, window_height/2-player_size,player_size,player_size},
        .is_diving = false
      },
      {}
    },
    .score = 0
  };
  if(initialize_game(&game,renderer) != 0){
    destroy_game(&game);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return EXIT_FAILURE;
  };
  Uint32 time_per_frame = 1000 / game.game_settings.FPS;
  Uint32 start_time = SDL_GetTicks() - time_per_frame;
  Uint32 end_time;
  Uint32 delta_ms;

  while (game.state != STATE_QUIT) {
    end_time = SDL_GetTicks();
    delta_ms = end_time - start_time;
    if (delta_ms > time_per_frame) {
      process_events(&game);
      update(&game, delta_ms);
      draw(renderer, game.game_objects, game.spritesheets,&game.messages,game.display_settings);
      start_time = end_time;
    } else {
      SDL_Delay(time_per_frame - delta_ms);
    }
  }
  destroy_game(&game);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  return EXIT_SUCCESS;
}
