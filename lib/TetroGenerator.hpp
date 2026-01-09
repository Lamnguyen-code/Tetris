#pragma once

#include "Tetromino.hpp"
#include <vector>
#include <random>
#include <algorithm>

class TetroGenerator {
private:
    std::vector<int> m_vec;
    int m_curIdx;
    std::random_device m_rd;
    std::default_random_engine m_rng;
public:
    TetroGenerator() {
        m_vec = {0, 1, 2, 3, 4, 5, 6};
        m_curIdx = 0;

        // Define random device
        m_rng = std::default_random_engine(m_rd()); 
        // Shuffle m_vec
        std::shuffle(m_vec.begin(), m_vec.end(), m_rng);
    }

    ~TetroGenerator() {};

    Tetromino generateTetro() {
        if (m_curIdx == m_vec.size()) {
            m_curIdx = 0;
            std::shuffle(m_vec.begin(), m_vec.end(), m_rng);
        }

        return Tetromino(m_vec[m_curIdx ++]);
    }
};
