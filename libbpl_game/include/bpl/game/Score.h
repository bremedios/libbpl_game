//
// Created by Bradley Remedios on 11/22/24.
//

#ifndef BPL_GAME_SCORE_H
#define BPL_GAME_SCORE_H

#include <string>
#include <map>
#include <memory>

namespace bpl::game {
    class Score {
    public:
        static Score& getInstance();

        Score() = default;
        ~Score() = default;

        void StartLevel(int level);

        bool Achievement(const std::string& achievement);
        bool AddPointMap(const std::string& achievement, int points);

        int getScore() const {return m_score;}

    private:
        struct LevelScore {
            int totalPoints;
            std::map<std::string, int> counts;
        };

        typedef std::shared_ptr<LevelScore> LevelScorePtr;

        typedef std::map<int, LevelScorePtr> LevelMap;
        typedef std::map<std::string, int> PointsMap;

        PointsMap m_points;
        LevelMap m_levels;
        int m_level=0;
        int m_score=0;
    }; // Score

} // bpl::game

#endif //BPL_GAME_SCORE_H