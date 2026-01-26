#include "constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

struct ball {
  float x;
  float y;
  float width;
  float height;
} ball;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int game_is_running = FALSE;
int initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "[ERROR] - Error initializing SDL.\n");
    return FALSE;
  }
  window = SDL_CreateWindow("SDL Pong", SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                            SDL_WINDOW_RESIZABLE);
  if (window == NULL) {
    fprintf(stderr, "[ERROR] - Error creating Window.\n");
    return FALSE;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (renderer == NULL) {
    fprintf(stderr, "[ERROR] - Error creating renderer.\n");
    return FALSE;
  }
  return TRUE;
}
void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_QUIT:
    game_is_running = FALSE;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE)
      game_is_running = FALSE;
    break;
  }
}
void update(void) {}
void render(void) {
  SDL_SetRenderDrawColor(renderer, 137, 43, 225, 255);
  SDL_RenderClear(renderer);
  SDL_Rect ball_rectangle = {ball.x, ball.y, ball.width, ball.height};
  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
  SDL_RenderFillRect(renderer, &ball_rectangle);
  SDL_RenderPresent(renderer);
}
void cleanup(void) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
void setup_game(void) {
  ball.x = 20.0f;
  ball.y = 20.0f;
  ball.width = 15.0f;
  ball.height = 15.0f;
}
int main() {
  game_is_running = initialize_window();
  setup_game();
  while (game_is_running) {
    process_input();
    update();
    render();
  }
  cleanup();
  return 0;
}
