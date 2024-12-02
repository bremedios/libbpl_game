//
// Created by Bradley Remedios on 11/7/24.
//

#ifndef BPL_GAME_PLATFORMERENEMYTOKEN_H
#define BPL_GAME_PLATFORMERENEMYTOKEN_H

#include <bpl/game/PlatformerToken.h>

namespace bpl::game {
    class PlatformerEnemyToken : public bpl::game::PlatformerToken{
    public:
        PlatformerEnemyToken();
        ~PlatformerEnemyToken() override;

        void Action();
    private:
        int m_xDirection = 0;
        int m_yDirection = 0;

        bool m_freshLadderChoice = false;
    };

    typedef std::shared_ptr<PlatformerEnemyToken> PlatformerEnemyTokenPtr;
    typedef std::list<PlatformerEnemyTokenPtr>  PlatformerEnemyTokenList;

} // bpl::game

#endif //BPL_GAME_PLATFORMERENEMYTOKEN_H