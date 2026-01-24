#include "../lib/Game.hpp"
#include "../lib/TetroGenerator.hpp"
#include <SDL2/SDL_timer.h>

Game::Game() {
    m_lines = m_scores = 0;
    m_hasTetro = false;

    m_gameState = INTRO;
    m_running = true;
    SDL_Init(SDL_INIT_VIDEO);

    m_level = 1;
    m_lastTime = 0;
    m_timer = 0;
    m_dropInterval = 1000;
    m_tmpLines = 0;

    // initialize next tetrominos
    m_nextTetros.resize(3);
    for (int i = 0; i < 3; ++i) 
        m_nextTetros[i] = m_tetroGenerator.generateTetro();
}

Game::~Game() {
    SDL_Quit();
}

void Game::handlePlayingInput() {
    // moveleft
    if (m_inpHandler.isActiveAction(MOVELEFT)) {
        m_tetro.move(-1, 0);
        if (m_board.isCollision(m_tetro) != 1)
            m_tetro.move(1, 0);
    }
   
    // moveright
    else if (m_inpHandler.isActiveAction(MOVERIGHT)) {
        m_tetro.move(1, 0);
        if (m_board.isCollision(m_tetro) != 1)
            m_tetro.move(-1, 0);
    }
    
    // movedown
    else if (m_inpHandler.isActiveAction(MOVEDOWN)) {
        m_tetro.move(0, 1);
        if (m_board.isCollision(m_tetro) != 1)
            m_tetro.move(0, -1);
    }

    // fall immediately
    else if (m_inpHandler.isActiveAction(FALL)) {
        m_tetro.copyPos(m_shadow);
        if (isLost()) return;

        // add this tetro to board
        m_board.addTetro(m_tetro);
        m_hasTetro = false;
    }

    // rotate
    else if (m_inpHandler.isActiveAction(ROTATE)) {
        m_tetro.rotate();
        if (m_board.isCollision(m_tetro) != 1) { // can't rotate
            m_tetro.move(-1, 0); // move left 1 cell
            if (m_board.isCollision(m_tetro) != 1) { // move left 1 cell also can't rotate
                m_tetro.move(2, 0); // move right 1 cell, but it has moved 1 cell left already so now it need to move 2 cell to the right

                if (m_board.isCollision(m_tetro) != 1) { // move right 1 cell also can't rotate, need to undorotate
                    m_tetro.move(-1, 0);
                    m_tetro.undoRotate();
                }
            }
        } 
    }
}

void Game::updateShadow() {
    m_shadow = m_tetro;
    bool isValid;
    do {
        m_shadow.move(0, 1);
        isValid = m_board.isCollision(m_shadow);
        if (!isValid)
            m_shadow.move(0, -1);
    } while (isValid);
}

void Game::dropTetro() {
    Uint32 currentTime = SDL_GetTicks();
    int deltaTime = int(currentTime - m_lastTime);

    // update m_lastTime
    m_lastTime = currentTime;

    m_timer += deltaTime; 
    if (m_timer >= m_dropInterval) {
        m_timer -= m_dropInterval;
        m_tetro.move(0, 1);
    }
}

void Game::checkLevelUp() {
    if (m_tmpLines >= 10) {
        m_tmpLines = 0;

        // uplevel 
        ++m_level;
        
        // decrease drop interval
        m_dropInterval = std::max(m_dropInterval - 100, 100);
    }
}

bool Game::isLost() const {
    std::vector<std::pair<int, int>> vec = m_tetro.getTetroPos();

    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i].second < 0) {
            return true;
        }
    }

    return false;
}

void Game::resetStatus() {
    // score
    m_lines = 0;
    m_scores = 0;

    // tetromino
    m_hasTetro = false;

    // timer
    m_timer = 0;
    m_dropInterval = 1000;

    // level
    m_level = 1;
    m_tmpLines = 0;

    // reset tetrominos
    m_tetroGenerator.reset();
    for (int i = 0; i < 3; ++i) {
        m_nextTetros[i] = m_tetroGenerator.generateTetro();

    // reset board
    m_board.reset();
    }
}

void Game::run() {
    while (m_running) {
        m_inpHandler.reset();
        m_inpHandler.update();
        // check quit
        m_running = !m_inpHandler.isQuit();

        // game logic
        switch(m_gameState) {
            case INTRO: {
                Button playButton = {150, 118, 200, 80, {76, 176, 88, 255}, {209, 122, 63, 255}, "Play", 50, {70, 70, 70, 255}};
                // Load assets
                m_renderer.loadIntroAssets(playButton);
                // Render intro
                m_renderer.renderIntro(playButton, m_scoreManager.getScores());

                // check hover button
                m_inpHandler.updateHoveredButton(playButton); 
                
                // check playButton is clicked
                if (m_inpHandler.isClickedButton(playButton)) {
                    m_gameState = PLAYING;
                    m_renderer.freeIntroAssets();
                }

                break;
            }
            case PLAYING:{
                // load assets
                m_renderer.loadPlayAssets();
                // generate tetromino
                if (!m_hasTetro) {
                    m_hasTetro = true;
                    m_tetro = m_nextTetros[0];
                    m_shadow = m_tetro;
                    updateShadow();

                    // update nextTetros
                    m_nextTetros[0] = m_nextTetros[1];
                    m_nextTetros[1] = m_nextTetros[2];
                    m_nextTetros[2] = m_tetroGenerator.generateTetro();
                    // start timing
                    m_lastTime = SDL_GetTicks();
                }
                
                // render
                m_renderer.renderPlay(m_board, m_tetro, m_shadow, m_nextTetros, m_level, m_scores, m_lines);

                // check input
                handlePlayingInput();

                // update shadow of current tetromino
                updateShadow();

                // tetro fall + check lost
                if (m_hasTetro) {
                    dropTetro();
                    if (m_board.isCollision(m_tetro) == 0) { // if there is collison
                        // first case, player lost
                        if (isLost()) {
                            m_gameState = GAMEOVER;
                            // free assets
                            m_renderer.freePlayAssets();
                            m_scoreManager.update(m_scores);
                        }
    
                        // second case, tetro can't move down anymore, so stick it to m_board
                        else {
                            m_tetro.move(0, -1);
    
                            // add this tetro to m_board
                            m_board.addTetro(m_tetro);
    
                            // modify m_hasTetro to generate a new tetromino
                            m_hasTetro = false;
                        }
                    }
                }

                // clear lines
                std::vector<int> completedLines = m_board.findCompletedLines();
                m_tmpLines += completedLines.size();

                // update score and lines
                m_scores += m_level * completedLines.size();
                m_lines += completedLines.size();
                
                // render completed lines
               
                // clear completed lines
                m_board.clearCompletedLines(completedLines);

                // check up level
                checkLevelUp();
                
                break;
            }

            default:
                Button replayButton = {150, 285, 200, 50, {76, 176, 88, 255}, {209, 122, 63, 255}, "Replay", 40, {70, 70, 70, 255}};
                
                Button exitButton = {150, 405, 200, 50, {76, 176, 88, 255}, {209, 122, 63, 255}, "Exit", 40, {70, 70, 70, 255}};

                m_renderer.loadGameOverAssets();
                m_renderer.renderGameOver(m_scores, replayButton, exitButton);
                
                // check mouse input
                m_inpHandler.updateHoveredButton(replayButton);
                m_inpHandler.updateHoveredButton(exitButton);

                if (m_inpHandler.isClickedButton(replayButton)) {
                    m_gameState = PLAYING;
                    m_renderer.freeGameOverAssets();
                    resetStatus();
                }

                else if (m_inpHandler.isClickedButton(exitButton))
                    m_running = false;
        }
    } 
}
