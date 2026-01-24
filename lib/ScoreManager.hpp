#pragma once

#include <vector>

class ScoreManager {
private:
    std::vector<int> m_scores;
public:
    ScoreManager();
    std::vector<int>& getScores();
    void update(int);
    ~ScoreManager();
};
