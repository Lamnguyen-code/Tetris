#pragma once

#include <SDL2/SDL.h>
#include <string>

struct Button : SDL_Rect {
    SDL_Color baseColor;
    SDL_Color hoveredColor;
    bool isHovered;
    std::string text;
    int textSize;
    SDL_Color textColor;
    
    Button() {};
    Button(int x, int y, int w, int h, const SDL_Color& b_color, const SDL_Color& h_color, const std::string& t, int tSize, const SDL_Color& color) : SDL_Rect{x, y, w, h}, baseColor(b_color), hoveredColor(h_color) , text(t), textSize(tSize), textColor(color) {};
};


