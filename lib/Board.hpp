#pragma once

#include "Tetromino.hpp"
#include <vector>
#include "config.hpp"

class Board {
private:
    std::vector<std::vector<int>> m_grid;
public:
    Board();

    int isCollision(const Tetromino&) const; // -1: reach side borders, 0: can't move down, 1: is okay
    void addTetro(const Tetromino&);
    std::vector<int> findCompletedLines() const;
    void clearCompletedLines(const std::vector<int>&);
    int getCellValue(int i, int j) const {return m_grid[i][j];}
};
