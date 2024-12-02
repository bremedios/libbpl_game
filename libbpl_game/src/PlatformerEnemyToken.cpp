//
// Created by Bradley Remedios on 11/7/24.
//

#include <bpl/game/PlatformerEnemyToken.h>

#include <bpl/graphics/collider/Ladder.h>

namespace bpl::game {
    PlatformerEnemyToken::PlatformerEnemyToken() = default;

    PlatformerEnemyToken::~PlatformerEnemyToken() = default;

    void PlatformerEnemyToken::Action() {
        //
        //  If it is our first move, we choose left or right
        //
        if ((m_xDirection == 0) && (m_yDirection == 0)) {
            if (std::rand() % 2 == 0) {
                m_xDirection = 1;
            }
            else {
                m_xDirection = -1;
            }

            //
            //  If we can't move, we chose to move into a wall and lets go the other way.
            //
            if (!Move(m_xDirection, m_yDirection)) {
                m_xDirection *= -1;
                Move(m_xDirection, m_yDirection);
            }

            return;
        }

        //
        //  If we are climbing or going up a ladder, we just keep doing that unless we are blocked.
        //
        if (m_yDirection != 0) {
            //
            //  We could move so we are done.
            //
            if (Move(m_xDirection, m_yDirection)) {
                return;
            }
        }

        bool canGoUp = false;
        bool canGoDown = false;
        bool canGoRight = false;
        bool canGoLeft = false;
        SDL_Rect collider = GetCurrentSprite()->GetCollider();

        //
        //  If we haven't already entered a ladder, then we check if we can climb up or down
        //
            //
            //  Determine if left or right are currently options for us.
            //
            for (auto & it : m_ladders) {
                if (bpl::graphics::collider::Ladder::CanClimbUpLadder(it->GetCollider(), collider)) {
                    canGoUp = true;
                }
                if (bpl::graphics::collider::Ladder::CanClimbDownLadder(it->GetCollider(), collider)) {
                    canGoDown = true;
                }
            }

        if ((canGoDown || canGoUp) && m_freshLadderChoice) {
            //
            //  Once we've gone in here, we've made our choice
            //
            m_freshLadderChoice = false;

            //
            //  If we are just going into a ladder, we can make a decision to change
            //
            switch (std::rand() % 3) {
                case 0:
                    if (canGoDown) {
                        // go down ladder
                        m_xDirection = 0;
                        m_yDirection = 1;
                        Move(m_xDirection, m_yDirection);
                        return;
                    }
                break;
                case 1:
                    if (canGoUp) {
                        // go up ladder
                        m_xDirection = 0;
                        m_yDirection = -1;
                        Move(m_xDirection, m_yDirection);
                        return;
                    }
                break;
                default:
                    // continue on.
                break;
            }
        } else if (!canGoDown && !canGoUp) {
            m_freshLadderChoice = true;
        }

        //
        //  We could be moving in any direction
        //
        if (!Move(m_xDirection, m_yDirection)) {
            switch (std::rand() % 4) {
            case 0:
                if (canGoDown) {
                    // go down ladder
                    m_xDirection = 0;
                    m_yDirection = 1;
                    Move(m_xDirection, m_yDirection);
                    return;
                }
            break;
            case 1:
                if (canGoUp) {
                    // go up ladder
                    m_xDirection = 0;
                    m_yDirection = -1;
                    Move(m_xDirection, m_yDirection);
                    return;
                }
                break;
                case 2:
                        //  Right
                        m_xDirection = 1;
                        m_yDirection = 0;
                        if (!Move(m_xDirection, m_yDirection)) {

                            m_xDirection = -1;
                            Move(m_xDirection, m_yDirection);
                        }
                        return;
                case 3:
                    //Left
                    m_xDirection = -1;
                    m_yDirection = 0;
                    if (!Move(m_xDirection, m_yDirection)) {

                        m_xDirection = 1;
                        Move(m_xDirection, m_yDirection);
                    }
                    return;
            default:
                // continue on.
                    break;
            }
        }
    } // Action

} // bpl::game