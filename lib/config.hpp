#pragma once

#include <string>

// Define font path
const std::string fontPath = "../assets/Font/Font.ttf";

// Define high scores path
const std::string scorePath = "../assets/highscores.txt";

// Define size of board (in cells)
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;

// Define size of screen (in pixels)
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 700;

// Game state
enum GameState {
    INTRO,
    PLAYING,
    GAMEOVER
};

// Tetromino actions
enum TetroAction {
    MOVELEFT,
    MOVERIGHT,
    MOVEDOWN,
    FALL,
    ROTATE
};
