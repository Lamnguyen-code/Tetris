#include "../lib/Renderer.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

std::vector<SDL_Color> Renderer::m_colorMap = {{130,0,200,180}, {200, 200, 40, 180}, {200, 0, 0, 180}, {0, 180, 0, 180}, {0, 180, 180, 180}, {210, 140, 0, 180}, {0, 60, 210, 180}};

Renderer::Renderer() {
    m_screen = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    m_renderer = SDL_CreateRenderer(m_screen, -1, SDL_RENDERER_PRESENTVSYNC);

    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    
    TTF_Init();
    m_font = nullptr;
    m_texture = nullptr;
}

Renderer::~Renderer() {
    IMG_Quit();
    TTF_Quit();

    if (m_font)
        TTF_CloseFont(m_font);
    m_font = nullptr;
    if (m_texture)
        SDL_DestroyTexture(m_texture); // clear m_texture
    m_texture = nullptr;

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_screen);
}

void Renderer::drawButton(const Button& button) {
    // define button color
    if (!button.isHovered) {
        SDL_SetRenderDrawColor(m_renderer, button.baseColor.r, button.baseColor.g, button.baseColor.b, button.baseColor.a);
    }

    else {
        SDL_SetRenderDrawColor(m_renderer, button.hoveredColor.r, button.hoveredColor.g, button.hoveredColor.b, button.hoveredColor.a);
    }

    SDL_RenderFillRect(m_renderer, &button); // fill buton with color
    
    // Draw border
    SDL_SetRenderDrawColor(m_renderer, 70, 70, 70, 255);
    for (int i = 1; i <= 3; ++i) {
        SDL_Rect border = {button.x - i, button.y - i, button.w + 2*i, button.h + 2*i};
        SDL_RenderDrawRect(m_renderer, &border);
    }

    SDL_Surface* textSurface = TTF_RenderText_Blended(m_font, button.text.c_str(), button.textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);

    // Text Rectangle
    int textW, textH;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);
    SDL_Rect textRec = {button.x + (button.w - textW) / 2, button.y + (button.h - textH) / 2, textW, textH};

    SDL_RenderCopy(m_renderer, textTexture, nullptr, &textRec);

    // Free memory
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// =========================== INTRO Functions ====================

void Renderer::loadIntroAssets(const Button& button) {
    // Define font
    if (!m_font)    
        m_font = TTF_OpenFont(fontPath.c_str(), button.textSize);

    if (!m_texture)
        m_texture = IMG_LoadTexture(m_renderer, "../assets/Images/IntroBG.jpg");
}

void Renderer::freeIntroAssets() {
    if (m_font)
        TTF_CloseFont(m_font);
    m_font = nullptr;
    if (m_texture)
        SDL_DestroyTexture(m_texture); // clear m_texture
    m_texture = nullptr;
}

void Renderer::renderIntro(const Button& playButton) {
    // SDL_SetRenderDrawColor(m_renderer, 69, 125, 198, 255);
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderClear(m_renderer); // clear background

    SDL_Rect dest = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}; // Intermediary rectangle

    SDL_RenderCopy(m_renderer, m_texture, nullptr, &dest); // copy m_texture into dest 

    // Draw play button
    drawButton(playButton); 

    SDL_RenderPresent(m_renderer); // display modifications
}

// ============================== PLAYING Functions ======================
void Renderer::loadPlayAssets() {
    if (!m_font)    
        m_font = TTF_OpenFont(fontPath.c_str(), 30);
}

void Renderer::freePlayAssets() {
    if (m_font) 
        TTF_CloseFont(m_font);
    m_font = nullptr;
}

void Renderer::renderGrid() const {
   
    // Draw board grid
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    for (int i = 0; i < BOARD_HEIGHT; ++i) { // draw horizontal lines
        SDL_RenderDrawLine(m_renderer, 30, 50 + i*30, 330, 50 + i*30);
    }

    for (int i = 0; i < BOARD_WIDTH; ++i) { // draw vertical lines
        SDL_RenderDrawLine(m_renderer, 30 + 30*i, 50, 30 + 30*i, 650);
    }
}

void Renderer::renderBoard(const Board& m_board) const {
    // Draw fixed tetrominos stick to grid
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if (m_board.getCellValue(i, j) == -1) continue; // empty cell

            SDL_Color cellColor = m_colorMap[m_board.getCellValue(i, j)];
            SDL_SetRenderDrawColor(m_renderer, cellColor.r, cellColor.g, cellColor.b, cellColor.a);

            SDL_Rect cellRect = {30 + 30*j, 50 + 30*i, 30, 30};
            SDL_RenderFillRect(m_renderer, &cellRect);
        }
    }
}

void Renderer::renderFallingTetro(const Tetromino& m_tetro) const {
    std::vector<std::pair<int, int>> posVec = m_tetro.getTetroPos();

    // get tetromino's color
    SDL_Color tetroColor = m_colorMap[m_tetro.getType()];
    SDL_SetRenderDrawColor(m_renderer, tetroColor.r, tetroColor.g, tetroColor.b, tetroColor.a);

    // Draw tetromino
    for (int i = 0; i < posVec.size(); ++i) {
        int x = posVec[i].first, y = posVec[i].second;
        if (y < 0) continue; // this cell is not in the board now

        SDL_Rect cellRect = {30 + 30*x, 50 + 30*y, 30, 30};

        SDL_RenderFillRect(m_renderer, &cellRect);
    }
}

void Renderer::renderNextTetros(const std::vector<Tetromino>& nextTetros) {
    // Draw rectangle
    SDL_SetRenderDrawColor(m_renderer, 70, 70, 70, 255);
    SDL_Rect tmpRect = {355, 120, 120, 200};
    SDL_RenderFillRect(m_renderer, &tmpRect);

    // Render text "Next"
    SDL_Color textColor = {183, 194, 208, 255};
    SDL_Rect nextRect = {355, 80, 120, 40};
    std::string nextText = "NEXT";
    SDL_Surface* nextTextSurface = TTF_RenderText_Blended(m_font, nextText.c_str(), textColor);

    SDL_Texture* nextTextTexture = SDL_CreateTextureFromSurface(m_renderer, nextTextSurface);

    // Text Rectangle
    int textW, textH;
    SDL_QueryTexture(nextTextTexture, nullptr, nullptr, &textW, &textH);
    SDL_Rect textRec = {nextRect.x + (nextRect.w - textW) / 2, nextRect.y + (nextRect.h - textH) / 2, textW, textH};

    SDL_RenderCopy(m_renderer, nextTextTexture, nullptr, &textRec);

    // Free memory
    SDL_FreeSurface(nextTextSurface);
    SDL_DestroyTexture(nextTextTexture);

    // Iterate through each next tetros
    for (int i = 0; i < nextTetros.size(); ++i) {
        int type = nextTetros[i].getType();
        
        // get color
        SDL_Color curColor = m_colorMap[type];
        SDL_SetRenderDrawColor(m_renderer, curColor.r, curColor.g, curColor.b, curColor.a);

        // position of tetro in next tetros board
        int posX = 1, posY = 3*i;
        if (type == 1 || type >= 5) ++posY; // o-tetromino, j + l -tetromino

        Tetromino tmpTetro = nextTetros[i];
        tmpTetro.setPos(posX, posY);
        std::vector<std::pair<int, int>> posVec = tmpTetro.getTetroPos();
        for (int j = 0; j < posVec.size(); ++j) {
            int x = posVec[j].first, y = posVec[j].second;
            
            SDL_Rect cellRect = {365 + 20*x, 120 + 20*y, 20, 20};
            SDL_RenderFillRect(m_renderer, &cellRect);
        }
    }
    
    SDL_SetRenderDrawColor(m_renderer, 70, 70, 70, 255);
    for (int i = 0; i < 5; ++i) { // vertical lines
        SDL_RenderDrawLine(m_renderer, 365 + i*20, 120, 365 + i*20, 300); 
    }
    for (int i = 0; i < 9; ++i) { // horizontal lines
        SDL_RenderDrawLine(m_renderer, 365, 120 + 20*i, 465, 120 + 20*i);
    }
}

void Renderer::renderLevel(int level) {
     // Draw rectangle
    SDL_SetRenderDrawColor(m_renderer, 70, 70, 70, 255);
    SDL_Rect tmpRect = {355, 380, 120, 40};
    SDL_RenderFillRect(m_renderer, &tmpRect);

    // Render text "Level"
    SDL_Color textColor = {183, 194, 208, 255};
    SDL_Rect levelRect = {355, 350, 120, 30};
    std::string levelText = "LEVEL";
    SDL_Surface* levelTextSurface = TTF_RenderText_Blended(m_font, levelText.c_str(), textColor);

    SDL_Texture* levelTextTexture = SDL_CreateTextureFromSurface(m_renderer, levelTextSurface);

    // Text Rectangle
    int textW, textH;
    SDL_QueryTexture(levelTextTexture, nullptr, nullptr, &textW, &textH);
    SDL_Rect textRec = {levelRect.x + (levelRect.w - textW) / 2, levelRect.y + (levelRect.h - textH) / 2, textW, textH};

    SDL_RenderCopy(m_renderer, levelTextTexture, nullptr, &textRec);

    // Free memory
    SDL_FreeSurface(levelTextSurface);
    SDL_DestroyTexture(levelTextTexture);

    std::string number = std::to_string(level);
    SDL_Surface* numberSurface = TTF_RenderText_Blended(m_font, number.c_str(), textColor);
    SDL_Texture* numberTexture = SDL_CreateTextureFromSurface(m_renderer, numberSurface);

    SDL_QueryTexture(numberTexture, nullptr, nullptr, &textW, &textH);
    SDL_Rect numberRec = {tmpRect.x + (tmpRect.w - textW) / 2, tmpRect.y + (tmpRect.h - textH) / 2, textW, textH};

     SDL_RenderCopy(m_renderer, numberTexture, nullptr, &numberRec);

    // Free memory
    SDL_FreeSurface(numberSurface);
    SDL_DestroyTexture(numberTexture);
} 

void Renderer::renderScore(int score) {
     // Draw rectangle
    SDL_SetRenderDrawColor(m_renderer, 70, 70, 70, 255);
    SDL_Rect tmpRect = {355, 470, 120, 40};
    SDL_RenderFillRect(m_renderer, &tmpRect);

    // Render text "Score"
    SDL_Color textColor = {183, 194, 208, 255};
    SDL_Rect scoreRect = {355, 440, 120, 30};
    std::string scoreText = "Score";
    SDL_Surface* scoreTextSurface = TTF_RenderText_Blended(m_font, scoreText.c_str(), textColor);

    SDL_Texture* scoreTextTexture = SDL_CreateTextureFromSurface(m_renderer, scoreTextSurface);

    // Text Rectangle
    int textW, textH;
    SDL_QueryTexture(scoreTextTexture, nullptr, nullptr, &textW, &textH);
    SDL_Rect textRec = {scoreRect.x + (scoreRect.w - textW) / 2, scoreRect.y + (scoreRect.h - textH) / 2, textW, textH};

    SDL_RenderCopy(m_renderer, scoreTextTexture, nullptr, &textRec);

    // Free memory
    SDL_FreeSurface(scoreTextSurface);
    SDL_DestroyTexture(scoreTextTexture);

    std::string number = std::to_string(score);
    SDL_Surface* numberSurface = TTF_RenderText_Blended(m_font, number.c_str(), textColor);
    SDL_Texture* numberTexture = SDL_CreateTextureFromSurface(m_renderer, numberSurface);

    SDL_QueryTexture(numberTexture, nullptr, nullptr, &textW, &textH);
    SDL_Rect numberRec = {tmpRect.x + (tmpRect.w - textW) / 2, tmpRect.y + (tmpRect.h - textH) / 2, textW, textH};

     SDL_RenderCopy(m_renderer, numberTexture, nullptr, &numberRec);

    // Free memory
    SDL_FreeSurface(numberSurface);
    SDL_DestroyTexture(numberTexture);
} 

void Renderer::renderLines(int lines) {
    // Render text "Lines"
    SDL_Color textColor = {183, 194, 208, 255};
    SDL_Rect linesRect = {355, 530, 120, 30};
    std::string linesText = "LINES";
    SDL_Surface* linesTextSurface = TTF_RenderText_Blended(m_font, linesText.c_str(), textColor);

    SDL_Texture* linesTextTexture = SDL_CreateTextureFromSurface(m_renderer, linesTextSurface);

    // Text Rectangle
    int textW, textH;
    SDL_QueryTexture(linesTextTexture, nullptr, nullptr, &textW, &textH);
    SDL_Rect textRec = {linesRect.x + (linesRect.w - textW) / 2, linesRect.y + (linesRect.h - textH) / 2, textW, textH};

    SDL_RenderCopy(m_renderer, linesTextTexture, nullptr, &textRec);

    // Free memory
    SDL_FreeSurface(linesTextSurface);
    SDL_DestroyTexture(linesTextTexture);

    // Draw rectangle
    SDL_SetRenderDrawColor(m_renderer, 70, 70, 70, 255);
    SDL_Rect tmpRect = {355, 560, 120, 40};
    SDL_RenderFillRect(m_renderer, &tmpRect);

    std::string number = std::to_string(lines);
    SDL_Surface* numberSurface = TTF_RenderText_Blended(m_font, number.c_str(), textColor);
    SDL_Texture* numberTexture = SDL_CreateTextureFromSurface(m_renderer, numberSurface);

    SDL_QueryTexture(numberTexture, nullptr, nullptr, &textW, &textH);
    SDL_Rect numberRec = {tmpRect.x + (tmpRect.w - textW) / 2, tmpRect.y + (tmpRect.h - textH) / 2, textW, textH};

     SDL_RenderCopy(m_renderer, numberTexture, nullptr, &numberRec);

    // Free memory
    SDL_FreeSurface(numberSurface);
    SDL_DestroyTexture(numberTexture);
}

void Renderer::renderStatus(int level, int score, int lines) {
    renderLevel(level);
    renderScore(score);
    renderLines(lines);
}

void Renderer::renderShadow(const Tetromino& shadow) const {
    SDL_SetRenderDrawColor(m_renderer, 90, 90, 90, 20);
    std::vector<std::pair<int, int>> posVec = shadow.getTetroPos();

    // Draw shadow
    for (int i = 0; i < posVec.size(); ++i) {
        int x = posVec[i].first, y = posVec[i].second;
        if (y < 0) continue; // this cell is not in the board now

        SDL_Rect cellRect = {30 + 30*x, 50 + 30*y, 30, 30};

        SDL_RenderFillRect(m_renderer, &cellRect);
    }
}

void Renderer::renderPlay(const Board& m_board, const Tetromino& m_tetro, const Tetromino& m_shadow, const std::vector<Tetromino>& nextTetros, int level, int lines, int score) {
    // Background
    SDL_SetRenderDrawColor(m_renderer, 62, 83, 110, 255);
    SDL_RenderClear(m_renderer);

    // Draw board rect
    SDL_Rect boardRect = {30, 50, 300, 600};
    SDL_SetRenderDrawColor(m_renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(m_renderer, &boardRect);

    // Draw board
    renderBoard(m_board);

    // Draw shadow
    renderShadow(m_shadow);

    // Draw falling tetromino
    renderFallingTetro(m_tetro);

    // Draw grid
    renderGrid();

    // Draw next tetros
    renderNextTetros(nextTetros);

    // Show status (level, lines, score)
    renderStatus(level, lines, score);

    // Show modifications
    SDL_RenderPresent(m_renderer);
}


// ============================= GAMEOVER Functions ========================
void Renderer::showScore(int score) {
    // Render text "Your Score"
    SDL_Color textColor = {29, 53, 87, 255};
    SDL_Rect textRect = {150, 100, 200, 50};
    std::string text = "Your Score";
    SDL_Surface* textSurface = TTF_RenderText_Blended(m_font, text.c_str(), textColor);

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);

    // Text Rectangle
    int textW, textH;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textW, &textH);
    SDL_Rect tmpRect = {textRect.x + (textRect.w - textW) / 2, textRect.y + (textRect.h - textH) / 2, textW, textH};

    SDL_RenderCopy(m_renderer, textTexture, nullptr, &tmpRect);

      // Draw rectangle
    SDL_SetRenderDrawColor(m_renderer, 110, 104, 44, 255);
    tmpRect = {150, 150, 200, 50};
    SDL_RenderFillRect(m_renderer, &tmpRect);


    // Free memory
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    textColor = {135, 143, 154, 255};
    std::string number = std::to_string(score);
    SDL_Surface* numberSurface = TTF_RenderText_Blended(m_font, number.c_str(), textColor);
    SDL_Texture* numberTexture = SDL_CreateTextureFromSurface(m_renderer, numberSurface);

    SDL_QueryTexture(numberTexture, nullptr, nullptr, &textW, &textH);
    SDL_Rect numberRec = {tmpRect.x + (tmpRect.w - textW) / 2, tmpRect.y + (tmpRect.h - textH) / 2, textW, textH};

     SDL_RenderCopy(m_renderer, numberTexture, nullptr, &numberRec);

    // Free memory
    SDL_FreeSurface(numberSurface);
    SDL_DestroyTexture(numberTexture);
}

void Renderer::loadGameOverAssets() {
    if (!m_font)
        m_font = TTF_OpenFont(fontPath.c_str(), 40);
}

void Renderer::freeGameOverAssets() {
    if (m_font) 
        TTF_CloseFont(m_font);
    m_font = nullptr;
}

void Renderer::renderGameOver(int score) {
    // background
    SDL_SetRenderDrawColor(m_renderer, 203, 203, 203, 255);
    SDL_RenderClear(m_renderer);

    // Draw score board
    showScore(score);

    // Show modifications
    SDL_RenderPresent(m_renderer);
}
