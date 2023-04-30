#include "score.h"
#include "game.h"
#include "./constants.h"

TTF_Font* initialize_font(const char* font_path, int font_size) {
    TTF_Font* font = TTF_OpenFont(font_path, font_size);
    if (!font) {
        fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
        return NULL;
    }
    return font;
}

void render_score_text(SDL_Renderer* renderer, TTF_Font* font) {
  char score_text[64];
  snprintf(score_text, sizeof(score_text), "%d  %d", player1.score, player2.score);

  SDL_Color text_color = {255, 255, 255, 255};
  SDL_Surface* text_surface = TTF_RenderText_Solid(font, score_text, text_color);
  if (!text_surface) {
    fprintf(stderr, "Error creating text surface: %s\n", TTF_GetError());
    return;
  }

  SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
  if (!text_texture) {
    fprintf(stderr, "Error creating text texture: %s\n", SDL_GetError());
    SDL_FreeSurface(text_surface);
    return;
  }

  SDL_Rect text_rect = {
    (WINDOW_WIDTH - text_surface->w) / 2,
    10,
    text_surface->w,
    text_surface->h
  };

  SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

  SDL_DestroyTexture(text_texture);
  SDL_FreeSurface(text_surface);
}

void destroy_font(TTF_Font* font) {
    TTF_CloseFont(font);
}