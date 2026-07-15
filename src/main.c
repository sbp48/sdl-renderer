#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <wchar.h>

#define WINDOW_TITLE "sdl"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define IMAGE_FLAGS IMG_INIT_PNG

struct Game {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *background;
};
bool load_media(struct Game *game);
void game_clean(struct Game *game, int exit_status);

bool sdl_init(struct Game *game) {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    fprintf(stderr, "no sdl init succ: %s\n", SDL_GetError());
    return true;
  };
  
  int img_init = IMG_Init(IMAGE_FLAGS);
  if ((img_init & IMAGE_FLAGS) != IMAGE_FLAGS) { 
    fprintf(stderr, "error img init: %s\n", SDL_GetError());
    return true;
  }

  game->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (!game->window) {
    fprintf(stderr, "err creating sdl window: %s\n", SDL_GetError());
    return true;
  };

  game->renderer = SDL_CreateRenderer(game->window, -1, 0);  
  if (!game->renderer) {
    fprintf(stderr, "err creating sdl renderer: %s\n", SDL_GetError());
    return true;
  };

  return false;
};

int main() {
  struct Game game = {
    .window = NULL,
    .renderer = NULL,
    .background = NULL,
  };
  
  if (sdl_init(&game)) {
    game_clean(&game, EXIT_FAILURE);
    printf("cleaning, exiting");
  }

  if (load_media(&game)) {
    game_clean(&game, EXIT_FAILURE);
    printf("cleaning, exiting");
  }
  
  while (true) {
    // sdl will check if user quits every frame (is this good?)
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          game_clean(&game, EXIT_SUCCESS);
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
              game_clean(&game, EXIT_SUCCESS);
              break;
            default:
              break;
          }
        default:
          break;
      }
    }

    SDL_RenderClear(game.renderer);

    SDL_RenderCopy(game.renderer, game.background, NULL, NULL);

    SDL_RenderPresent(game.renderer);
    SDL_Delay(16);
  }

  game_clean(&game, EXIT_SUCCESS);
  printf("good, exiting :)");
  return 0;
}

//loads anime
bool load_media(struct Game *game) {
  game->background = IMG_LoadTexture(game->renderer, "res/image.png");
  if (!game->background) { 
    fprintf(stderr, "error creating texture: %s\n", SDL_GetError());
    return true;
  }
  return false;
}

// free the memory when exit
void game_clean(struct Game *game, int exit_status) {
  SDL_DestroyTexture(game->background);
  SDL_DestroyRenderer(game->renderer);
  SDL_DestroyWindow(game->window);
  IMG_Quit();
  SDL_Quit();
  exit(exit_status);
}


