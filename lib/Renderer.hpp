#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Board.hpp"
#include "Tetromino.hpp"
#include "config.hpp"

#include "Button.hpp"

class Renderer {
public:
    static std::vector<SDL_Color> m_colorMap;
private:
    SDL_Window* m_screen;
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;
    TTF_Font* m_font;
public:
    Renderer();
    ~Renderer();

    void drawButton(const Button&); 

    // when GameState = INTRO
    void loadIntroAssets(const Button&);
    void freeIntroAssets();
    void renderIntro(const Button&); 

    // when GameState = PLAY
    void loadPlayAssets();
    void freePlayAssets();
    void renderPlay(const Board&, const Tetromino&, const Tetromino&, const std::vector<Tetromino>&, int, int, int); 
    void renderGrid() const;
    void renderBoard(const Board&) const;
    void renderFallingTetro(const Tetromino&) const;
    void renderShadow(const Tetromino&) const;
    void renderCompletedLines();
    void renderNextTetros(const std::vector<Tetromino>&);
    void renderLevel(int);
    void renderScore(int);
    void renderLines(int);
    void renderStatus(int, int, int); // level, scores, lines
    // when GameState = GAMEOVER
    void showScore(int);
    void loadGameOverAssets();
    void freeGameOverAssets();
    void renderGameOver(int);
    // void renderGameOver(); 
};
