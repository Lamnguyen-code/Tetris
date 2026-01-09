#include "../lib/InputHandler.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>

std::vector<SDL_Keycode> InputHandler::m_tetroActionMap = {SDLK_LEFT, SDLK_RIGHT, SDLK_DOWN, SDLK_SPACE, SDLK_UP}; // LEFT: moveleft, RIGHT: moveright, DOWN: movedown 1 cell, SPACE: fall down immediately, UP: rotate

InputHandler::InputHandler() {
    m_quit = false;
    m_isClicked = false;
    m_lastKey = SDLK_UNKNOWN;

}

void InputHandler::reset() {
    m_isClicked = false;
    m_lastKey = SDLK_UNKNOWN;
}

void InputHandler::update() {
    while (SDL_PollEvent(&m_env)) {
    // SDL_PollEvent(&m_env);
        // check quit
        if (m_env.type == SDL_QUIT)
            m_quit = true;

        else if (m_env.type == SDL_KEYDOWN) { // key pressed
            m_lastKey = m_env.key.keysym.sym;
        }

        else if (m_env.type == SDL_MOUSEBUTTONDOWN) {
            if (m_env.button.button == SDL_BUTTON_LEFT) {
                m_isClicked = true;
                SDL_GetMouseState(&m_x, &m_y);
            }
        }
        else {
            SDL_GetMouseState(&m_x, &m_y);
        }
    }
}

bool InputHandler::isQuit() {
    return m_quit;
}

void InputHandler::updateHoveredButton(Button& button) {
    SDL_Point mousePoint = {m_x, m_y};
    if (SDL_PointInRect(&mousePoint, &button))
        button.isHovered = true;
    else 
        button.isHovered = false;
}

bool InputHandler::isClickedButton(const Button& button) {
    if (m_isClicked) {
        SDL_Point clickPoint = {m_x, m_y};
        if (SDL_PointInRect(&clickPoint, &button))
            return true;
    }

    return false;
}

bool InputHandler::isActiveAction(const TetroAction& ta) const {
    return (m_lastKey == m_tetroActionMap[ta]);
}
