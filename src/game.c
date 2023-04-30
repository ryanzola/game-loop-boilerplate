#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "./constants.h"

struct ball ball;
struct player player1, player2;

void game_setup() {
  // Seed the random number generator with the current time
  srand(time(NULL));

  // Ball initialization - start in the center of the screen
  ball.x = (WINDOW_WIDTH / 2) - (ball.width / 2);
  ball.y = (WINDOW_HEIGHT / 2) - (ball.height / 2);
  ball.width = 15;
  ball.height = 15;

  // 45 degrees
  float angle = (rand() % 2 == 0 ? 45 : 135) * (M_PI / 180.0);

  // Set the magnitude of the velocity
  float velocity_magnitude = 300;

  // Calculate the x and y components of the velocity
  ball.velocity_x = cos(angle) * velocity_magnitude;
  ball.velocity_y = sin(angle) * velocity_magnitude;

  // Player 1 initialization
  player1.x = 10;
  player1.y = WINDOW_HEIGHT / 2 - 40;
  player1.width = 15;
  player1.height = 80;
  player1.velocity = 0;

  // Player 2 (opponent) initialization
  player2.x = WINDOW_WIDTH - 25;
  player2.y = WINDOW_HEIGHT / 2 - 40;
  player2.width = 15;
  player2.height = 80;
  player2.velocity = 0;
}

void set_ball() {
  // Reset the ball to the center of the screen
  ball.x = WINDOW_WIDTH / 2 - 7.5;
  ball.y = WINDOW_HEIGHT / 2 - 7.5;

  // either + or - 45 degrees
  float angle = (rand() % 2 == 0 ? 45 : 135) * (M_PI / 180.0);

  // Randomly reflect the angle horizontally
  if (rand() % 2 == 0) {
    angle = -angle;
  }

  // Set the magnitude of the velocity
  float velocity_magnitude = 300;

  // Calculate the x and y components of the velocity
  ball.velocity_x = cos(angle) * velocity_magnitude;
  ball.velocity_y = sin(angle) * velocity_magnitude;
}

void process_game_input(SDL_Event event) {
  switch (event.type) {
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_UP)
        player1.velocity -= 300;
      if (event.key.keysym.sym == SDLK_DOWN)
        player1.velocity += 300;
      break;
    case SDL_KEYUP:
      if (event.key.keysym.sym == SDLK_UP)
        player1.velocity = 0;
      if (event.key.keysym.sym == SDLK_DOWN)
        player1.velocity = 0;
      break;
  }
}

void handle_collisions() {
  // Check for collisions with left and right window bounds
  if (ball.x <= 0) {
    // Ball hit the left wall, player 2 (opponent) scored
    player2.score++;

    // Reset the ball to the center of the screen
    set_ball();
  } else if (ball.x + ball.width >= WINDOW_WIDTH) {
    // Ball hit the right wall, player 1 scored
    player1.score++;

    // Reset the ball to the center of the screen
    set_ball();
  }

  // Check for collisions with window bounds (top and bottom)
  if (ball.y <= 0 || ball.y + ball.height >= WINDOW_HEIGHT) {
    if (ball.y <= 0) {
      ball.y = 0;
    } else {
      ball.y = WINDOW_HEIGHT - ball.height;
    }
    ball.velocity_y = -ball.velocity_y;
  }

  // Check for collisions with players
  if (ball.x <= player1.x + player1.width &&
      ball.x >= player1.x &&
      ball.y <= player1.y + player1.height &&
      ball.y >= player1.y) {
    ball.x = player1.x + player1.width;
    ball.velocity_x = -ball.velocity_x;

    // Increase the velocity slightly to make the game more challenging
    ball.velocity_x *= 1.05;
  }

  if (ball.x + ball.width >= player2.x &&
      ball.x <= player2.x + player2.width &&
      ball.y <= player2.y + player2.height &&
      ball.y >= player2.y) {
    ball.x = player2.x - ball.width;
    ball.velocity_x = -ball.velocity_x;

    // Increase the velocity slightly to make the game more challenging
    ball.velocity_x *= 1.05;
  }
}

void update_player_movement(float delta_time) {
  // move the player1 as a function of delta time
  player1.y += player1.velocity * delta_time;

  // Limit player1's movement to stay within the bounds of the screen
  if (player1.y < 0) {
    player1.y = 0;
  } else if (player1.y + player1.height > WINDOW_HEIGHT) {
    player1.y = WINDOW_HEIGHT - player1.height;
  }
}

void update_ai(float delta_time) {
  // Target y position for player 2 (center of the ball)
  float target_y = ball.y + ball.height / 2 - player2.height / 2;

  // Linear interpolation (lerp) factor
  float lerp_factor = 4.5f;

  // Calculate the new y position for player 2 using lerp
  player2.y += (target_y - player2.y) * lerp_factor * delta_time;

  // Limit player2's movement to stay within the bounds of the screen
  if (player2.y < 0) {
    player2.y = 0;
  } else if (player2.y + player2.height > WINDOW_HEIGHT) {
    player2.y = WINDOW_HEIGHT - player2.height;
  }
}

void update_game(float delta_time) {
  // move the ball as a function of delta time
  ball.x += ball.velocity_x * delta_time;
  ball.y += ball.velocity_y * delta_time;

  // move the player1 as a function of delta time
  update_player_movement(delta_time);

  // Update the AI (player 2) movement
  update_ai(delta_time);

  handle_collisions();
}

void render_game(SDL_Renderer* renderer) {
  // draw a ball
  SDL_Rect ball_rect = {
    (int)ball.x,
    (int)ball.y,
    (int)ball.width,
    (int)ball.height
  };

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &ball_rect);

  // Draw player 1
  SDL_Rect player1_rect = {
    (int)player1.x,
    (int)player1.y,
    (int)player1.width,
    (int)player1.height
  };

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &player1_rect);

  // Draw player 2 (opponent)
  SDL_Rect player2_rect = {
    (int)player2.x,
    (int)player2.y,
    (int)player2.width,
    (int)player2.height
  };

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(renderer, &player2_rect);
}