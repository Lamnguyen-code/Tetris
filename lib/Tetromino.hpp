#pragma once

#include <vector>
#include <utility>
#include <random>
#include <iostream>

class Tetromino {
public:
    static std::vector<std::vector<std::vector<bool>>> shapeMap;

private:
    std::vector<std::vector<bool>> m_matrix;
    int m_x, m_y;
    int m_type;
public:
    Tetromino();
    Tetromino(int);

    void rotate();
    void undoRotate();
    void move(int, int);
    int getType() const;
    void setPos(int, int);
    void copyPos(const Tetromino&); // copy position of that tetromino
    std::vector<std::pair<int, int>> getTetroPos() const; // first: x, second: y
};
