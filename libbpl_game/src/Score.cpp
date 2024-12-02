//
// Created by Bradley Remedios on 11/22/24.
//
#include <memory>
#include <ranges>

#include <bpl/game/Score.h>

#include "Debug.h"

namespace bpl::game {
    void Score::StartLevel(int level) {
        std::shared_ptr<LevelScore> score;

        if (!m_levels.contains(level)) {
            score = std::make_shared<LevelScore>();

            m_levels.emplace(level, score);
        }
        else
        {
            score = m_levels[level];
        }

        score->totalPoints = 0;

        //  Initialize all achievements
        for (auto & it : std::views::keys(m_points)) {
            score->counts.emplace(it, 0);
        }

        m_level = level;
    } // StartLevel

    bool Score::Achievement(const std::string& achievement) {
        if (!m_levels.contains(m_level)) {
            ERROR_MSG("Level has not been started");

            return false;
        }

        auto level = m_levels[m_level];

        //  We add achievements as we receive them.
        if (!level->counts.contains(achievement)) {
            level->counts.emplace(achievement, 1);
        }
        else {
            level->counts[achievement] = level->counts[achievement] + 1;
        }

        //  We could have an achievement that has no points.  This is okay.
        if (m_points.contains(achievement)) {
            level->totalPoints += m_points[achievement];

            m_score += m_points[achievement];
        }

        return true;
    } // Achievement

    bool Score::AddPointMap(const std::string& achievement, int points) {
        if (!m_points.contains(achievement)) {
            m_points.emplace(achievement, points);
        }
        else {
            m_points[achievement] = points;
        }

        return true;
    } // AddPointMap

    Score& Score::getInstance() {
        static Score score;

        return score;
    } // getInstance
} // bpl::game