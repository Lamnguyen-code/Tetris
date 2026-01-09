#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "Button.hpp"
#include "config.hpp"

class InputHandler {
public:
    static std::vector<SDL_Keycode> m_tetroActionMap;
private:
    SDL_Event m_env;
    bool m_quit; // check if player want to quit game
    int m_x, m_y; // position of mouse
    bool m_isClicked;
    SDL_Keycode m_lastKey;
public:
    InputHandler();
    ~InputHandler() {};

    void reset(); // reset input state
    void update(); // update input state
    bool isQuit(); // check quit
    void updateHoveredButton(Button&); // update hover state of button
    bool isClickedButton(const Button&); // check if button is clicked

    bool isActiveAction(const TetroAction&) const; // check if the action is active
}; 
