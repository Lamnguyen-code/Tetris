#pragma once

#include "Board.hpp"
#include "InputHandler.hpp"
#include "Renderer.hpp"
#include "TetroGenerator.hpp"
#include "Tetromino.hpp"
#include "config.hpp"

class Game {
private:
    Board m_board;
    Renderer m_renderer;
    InputHandler m_inpHandler;
    Tetromino m_fallingTetro;
    GameState m_gameState;

    bool m_running; // bool variable for game loop

    int m_lines; // count number of completed lines
    int m_scores; // current score

    TetroGenerator m_tetroGenerator; // tetromino generator 
    Tetromino m_tetro; // current tetromino
    Tetromino m_shadow; // shadow of m_tetro
    bool m_hasTetro;
    std::vector<Tetromino> m_nextTetros; // vector of next tetrominos

    // Timer and level
    int m_level; // current level
    Uint32 m_lastTime; // the last time updated
    int m_timer; // accumulate timer
    int m_dropInterval; // interval time between 2 times the tetro drop
    int m_tmpLines; // temp counter for level up
public:
    Game();
    ~Game();

    void run(); // main function which contains the logic loop
    void handlePlayingInput(); // handle input when playing
    void dropTetro(); // drop tetro down
    void checkLevelUp();
    bool isLost() const;
    void updateShadow(); // update shadow if m_tetro has modification
};
