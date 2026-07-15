#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

#define WINDOW_TITLE "sdl"
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400

struct Game {
  SDL_Window *window;
  SDL_Renderer *renderer;
};

void game_clean(struct Game *game);

bool sdl_init(struct Game *game) {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    fprintf(stderr, "no sdl init succ: %s\n", SDL_GetError());
    return true;
  };

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
  };
  
  if (sdl_init(&game)) {
    game_clean(&game);
    printf("cleaning, exiting");
    exit(1);
  }

  SDL_RenderClear(game.renderer);
  SDL_RenderPresent(game.renderer);
  SDL_Delay(3000);
  game_clean(&game);
  printf("good, exiting :)");
  return 0;
}

// free the memory when exit
void game_clean(struct Game *game) {
  SDL_DestroyRenderer(game->renderer);
  SDL_DestroyWindow(game->window);
  SDL_Quit();
}


