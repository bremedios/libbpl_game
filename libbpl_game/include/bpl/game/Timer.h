//
// Created by Bradley Remedios on 11/15/24.
//

#ifndef BPL_GAME_TIMER_H
#define BPL_GAME_TIMER_H
#include <chrono>

namespace bpl::game {

    class Timer {
    public:
        Timer() = default;
        ~Timer() = default;

        void Start(int milliseconds);
        bool IsExpired();
        int SecondsLeft();
        int MillisecondsLeft();
    private:
        std::chrono::milliseconds m_expires;
    };

} // game

#endif //BPL_GAME_TIMER_H