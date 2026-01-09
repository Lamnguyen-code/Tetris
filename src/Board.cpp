#include "../lib/Board.hpp"

Board::Board() {
    m_grid = std::vector<std::vector<int>>(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, -1));
}

int Board::isCollision(const Tetromino& tetro) const {
    std::vector<std::pair<int, int>> cellPos = tetro.getTetroPos();
    int n = cellPos.size();

    // check each cells
    for (int i = 0; i < n; ++i) {
        int x = cellPos[i].first, y = cellPos[i].second;

        if (x < 0 || x >= BOARD_WIDTH) // cross side borders
            return -1;

        if (y < 0) continue;

        if (y >= BOARD_HEIGHT || m_grid[y][x] != -1) // can't move down anymore
            return 0;
    }

    // if none of previous conditions happend, there isn't collision
    return 1;
}

void Board::addTetro(const Tetromino& tetro) {
    std::vector<std::pair<int, int>> cellPos = tetro.getTetroPos();
    int n = cellPos.size();

    // Add each cells to grid
    for (int i = 0; i < n; ++i) {
        int x = cellPos[i].first, y = cellPos[i].second;
        m_grid[y][x] = tetro.getType();
    }
}

std::vector<int> Board::findCompletedLines() const {
    std::vector<int> res;
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        bool isFull = true;
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if (m_grid[i][j] == -1) { // an empty cell
                isFull = false;
                break;
            }
        }

        if (isFull) res.push_back(i);
    }

    return res;
}

// helper function
bool existedInVec(const std::vector<int>& vec, int val) {
    int n = vec.size();
    for (int i = 0; i < n; ++i) {
        if (vec[i] == val)
            return true;
    }

    return false;
}

void Board::clearCompletedLines(const std::vector<int>& completedLines) {
    int n = completedLines.size();
    if (n == 0) return; // there aren't completed lines

    int i = completedLines[n - 1], j = i - 1;
    // Use 2 pointers
    while (j >= 0) {
        if (!existedInVec(completedLines, j)) { // j is not a completed line
            m_grid[i--] = m_grid[j];
        }       

        --j;
    }

    // Process remaining lines
    while (i >= 0) {
        for (int j = 0; j < BOARD_WIDTH; ++j)
            m_grid[i][j] = -1;
        --i;
    }
}
