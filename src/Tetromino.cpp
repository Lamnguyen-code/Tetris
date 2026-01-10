#include "../lib/Tetromino.hpp"

// Define shape map of tetromino
std::vector<std::vector<std::vector<bool>>> Tetromino::shapeMap = {
        { // T-Tetromino
            {0, 0, 0},
            {1, 1, 1},
            {0, 1, 0}
        },

        { // O-Tetromino
            {1, 1},
            {1, 1}
        },

        { // Z-Tetromino
            {0, 0, 0},
            {1, 1, 0},
            {0, 1, 1}
        },

        { // S-Tetromino
            {0, 0, 0},
            {0, 1, 1},
            {1, 1, 0}
        },

        { // I-Tetromino
            {0, 0, 0, 0},
            {0, 0, 0, 0},
            {1, 1, 1, 1},
            {0, 0, 0, 0}
        },

        { // L-Tetromino
            {0, 0, 1},
            {1, 1, 1},
            {0, 0, 0}
        },

        { // J-Tetromino
            {1, 0, 0},
            {1, 1, 1},
            {0, 0, 0}
        }
};
Tetromino::Tetromino() {};

Tetromino::Tetromino(int type) {
    m_x = 4, m_y = -2; // position
    m_type = type;
    m_matrix = shapeMap[type];
}

int Tetromino::getType() const {
    return m_type;
}

void Tetromino::rotate() {
    int n = m_matrix.size();

    std::vector<std::vector<bool>> res(n, std::vector<bool>(n));

    for (int i = 0; i < n; ++i) { // rotate right
        for (int j = 0; j < n; ++j) {
            res[i][j] = m_matrix[n - 1 - j][i];
        }
    }

    m_matrix = res;
}

void Tetromino::undoRotate() {
    int n = m_matrix.size();

    std::vector<std::vector<bool>> res(n, std::vector<bool>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            res[i][j] = m_matrix[j][n - 1 - i];
    }

    m_matrix = res;
}

void Tetromino::move(int dx, int dy) {
    m_x += dx;
    m_y += dy;
}

std::vector<std::pair<int, int>> Tetromino::getTetroPos() const {
    int n = m_matrix.size();
    std::vector<std::pair<int, int>> res;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (m_matrix[i][j]) {
                res.push_back({m_x + j, m_y + i});
            }
        }
    }

    return res;
}

void Tetromino::setPos(int x, int y) {
    m_x = x, m_y = y;
}

void Tetromino::copyPos(const Tetromino& tetro) {
    m_x = tetro.m_x, m_y = tetro.m_y;
}
