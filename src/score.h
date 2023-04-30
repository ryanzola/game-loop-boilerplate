#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

TTF_Font* initialize_font(const char* font_path, int font_size);
void render_score_text(SDL_Renderer* renderer, TTF_Font* font);
void destroy_font(TTF_Font* font);