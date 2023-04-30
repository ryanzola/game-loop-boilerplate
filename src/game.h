#pragma once
#include <SDL.h>

struct ball {
    float x;
    float y;
    float width;
    float height;
    float velocity_x;
    float velocity_y;
};

struct player {
    float x;
    float y;
    float width;
    float height;
    float velocity;
    int score;
};

extern struct ball ball;
extern struct player player1, player2;

void game_setup();
void process_game_input(SDL_Event event);
void update_game(float delta_time);
void handle_collisions();
void update_player_movement(float delta_time);
void update_ai(float delta_time);
void reset_ball();
void render_game(SDL_Renderer* renderer);