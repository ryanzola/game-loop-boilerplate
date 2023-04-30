#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "./constants.h"
#include "./game.h"
#include "./score.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font;

int last_frame_time = 0;

int initialize_window() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return FALSE;
  }

  window = SDL_CreateWindow(
    NULL,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WINDOW_WIDTH,
    WINDOW_HEIGHT,
    SDL_WINDOW_BORDERLESS
  );

  if (!window) {
    fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
    return FALSE;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
    return FALSE;
  }

  if (TTF_Init() == -1) {
    fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
    return FALSE;
  }

  font = initialize_font("assets/pong-score.ttf", 64);
  if (!font) {
    fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
    return FALSE;
  }

  return TRUE;
}

void setup() {
  game_setup();
}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      game_is_running = FALSE;
      break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        game_is_running = FALSE;
      else
        process_game_input(event);
      break;
  }
}

void update() {
  // no need for this frame rate cap if calculating delta time
  // waste some time / sleep until we reach the target frame time length
  // while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME)); this takes 100% of the CPU 🔥
  // int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
  // if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
  //   SDL_Delay(time_to_wait);
  // }

  // delta time is the difference in ticks from last frame converted to seconds
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
  last_frame_time = SDL_GetTicks();

  update_game(delta_time);
}

void render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  render_game(renderer);
  render_score_text(renderer, font);

  SDL_RenderPresent(renderer);
}

void destroy() {
  destroy_font(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main() {
  game_is_running = initialize_window();

  setup();

  while (game_is_running) {
    process_input();
    update();
    render();
  }

  destroy();

  return 0;
}
