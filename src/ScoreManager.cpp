#include "../lib/ScoreManager.hpp"
#include "../lib/config.hpp"
#include <fstream>

ScoreManager::ScoreManager() {
    std::ifstream inp(scorePath);
    int tmp;
    while (inp >> tmp) {
        m_scores.push_back(tmp);
    }

    inp.close();
}

std::vector<int>& ScoreManager::getScores() {
    return m_scores;
}

void ScoreManager::update(int newScore) {
    m_scores.push_back(newScore);

    int t = newScore;
    int x = m_scores.size() - 2;
    while (x >= 0 && m_scores[x] < t) {
        m_scores[x + 1] = m_scores[x];
        --x;
    }

    m_scores[x + 1] = t;

    // remove redundant value
    if (m_scores.size() > 5) 
        m_scores.pop_back();
}

ScoreManager::~ScoreManager() {
    std::ofstream out(scorePath);

    for (int i = 0; i < m_scores.size(); ++i) 
        out << m_scores[i] << "\n";
}
