#include "constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
struct game_object {
  float x;
  float y;
  float width;
  float height;
  float velocity_x;
  float velocity_y;
} ball, paddle;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int game_is_running = FALSE;
static int previous_frame_time = 0;
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
    if (event.key.keysym.sym == SDLK_ESCAPE) {
      game_is_running = FALSE;
    }
    if (event.key.keysym.sym == SDLK_LEFT) {
      paddle.velocity_x = -400;
    }
    if (event.key.keysym.sym == SDLK_RIGHT) {
      paddle.velocity_x = 400;
    }
    break;
  case SDL_KEYUP:
    if (event.key.keysym.sym == SDLK_LEFT) {
      paddle.velocity_x = 0;
    }
    if (event.key.keysym.sym == SDLK_RIGHT) {
      paddle.velocity_x = 0;
    }
    break;
  }
}
void check_ball_collision() {
  // Wall
  if ((ball.x <= 0) || ((ball.x + ball.width) >= WINDOW_WIDTH)) {
    ball.velocity_x = -ball.velocity_x;
  }
  if (ball.y < 0) {
    ball.velocity_y = -ball.velocity_y;
  }
  // Paddle
  if ((ball.y + ball.height) >= paddle.y && (ball.x + ball.width) >= paddle.x &&
      ball.x <= (paddle.x + paddle.width)) {
    ball.velocity_y = -ball.velocity_y;
  }
}
void check_paddle_window_bounds() {
  if (paddle.x <= 0) {
    paddle.x = 0;
  }
  if (paddle.x >= (WINDOW_WIDTH - paddle.width)) {
    paddle.x = WINDOW_WIDTH - paddle.width;
  }
}
void check_game_over() {
  if (ball.y + ball.height > WINDOW_HEIGHT) {
    ball.x = WINDOW_WIDTH / 2.0f;
    ball.y = 0;
  }
}
void update(void) {
  int current_frame_time = SDL_GetTicks();
  int elapsed_time = current_frame_time - previous_frame_time;
  if (elapsed_time <= FRAME_TARGET_TIME) {
    SDL_Delay(elapsed_time);
  }
  float delta_time = (SDL_GetTicks() - previous_frame_time) / 1000.0f;
  previous_frame_time = SDL_GetTicks();
  ball.x += ball.velocity_x * delta_time;
  ball.y += ball.velocity_y * delta_time;
  paddle.x += paddle.velocity_x * delta_time;
  check_ball_collision();
  check_paddle_window_bounds();
  check_game_over();
}
void render(void) {
  SDL_SetRenderDrawColor(renderer, 137, 43, 225, 255);
  SDL_RenderClear(renderer);
  SDL_Rect ball_rectangle = {ball.x, ball.y, ball.width, ball.height};
  SDL_Rect paddle_rectangle = {paddle.x, paddle.y, paddle.width, paddle.height};
  SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
  SDL_RenderFillRect(renderer, &ball_rectangle);
  SDL_RenderFillRect(renderer, &paddle_rectangle);
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
  ball.velocity_x = 300.0f;
  ball.velocity_y = 300.0f;
  paddle.width = 100.0f;
  paddle.height = 20;
  paddle.x = (WINDOW_WIDTH / 2.0f) - (paddle.width / 2);
  paddle.y = WINDOW_HEIGHT - 40;
  paddle.velocity_x = 0;
  paddle.velocity_y = 0;
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
