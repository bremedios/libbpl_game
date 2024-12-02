//
// Created by bradr on 11/15/24.
//

#include <bpl/game/Timer.h>

namespace bpl::game {

    void Timer::Start(int milliseconds) {
        m_expires = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) + std::chrono::milliseconds(milliseconds);
    } // Start

    bool Timer::IsExpired() {
        if (m_expires <= std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch())) {
            return true;
        }

        return false;
    } // IsExpired

    int Timer::SecondsLeft() {
        return MillisecondsLeft() / 1000;
    } // SecondsLeft

    int Timer::MillisecondsLeft() {
        std::chrono::milliseconds timeLeft = std::chrono::duration_cast<std::chrono::milliseconds>(m_expires) - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

        return std::max(0, static_cast<int>(std::abs(timeLeft.count())));

    } // MillisecondsLeft


} // bpl::game