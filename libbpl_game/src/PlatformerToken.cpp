//
// Created by Bradley Remedios on 11/4/24.
//

#include <iostream>
#include <csignal>
#include <ranges>

#include <bpl/graphics/collider/Collider.h>
#include <bpl/graphics/collider/Floor.h>
#include <bpl/graphics/collider/Ladder.h>

#include <bpl/game/GameStateMachine.h>
#include <bpl/game/PlatformerToken.h>

//#define DEBUG 1

#if defined(DEBUG)
    #define DEBUG_FALL_COLLISIONS   1
    #define DEBUG_SPRITE_LOCATIONS  1
    #define DEBUG_COLLIDERS         1

    #define DEBUG_CHECK {std::raise(SIGINT);}
#else
    #define DEBUG_CHECK
#endif

#if defined(DEBUG_FALL_COLLISIONS)
    #define CHECK_FALL_COLLISIONS {                                                                                         \
            for (auto& spriteIt : m_sprites) {                                                                              \
                for (auto& it : m_floors) {                                                                                 \
                    if (collider::Collider::IsColliding(it->GetCollider(), m_sprites[spriteIt.first]->GetCollider())) {     \
                        std::cerr << "Floor Collision in " << __FUNCTION__ << std::endl;                                    \
                        std::raise(SIGINT);                                                                                 \
                    }                                                                                                       \
                }                                                                                                           \
            }                                                                                                               \
        }
#else
    #define CHECK_FALL_COLLISIONS
#endif // DEBUG_FALL_COLLISIONS

#if defined(DEBUG_COLLIDERS)
    #define CHECK_COLLIDERS {                                                                           \
        for (auto & spriteIt : m_sprites) {                                                             \
            int x =0;int y=0;                               \
            spriteIt.second->GetPosition(x,y);              \
            int currX=0;int currY=0;                        \
            GetCurrentSprite()->GetPosition(currX,currY);   \
            SDL_Rect currentCollider = GetCurrentSprite()->GetCollider();                               \
            SDL_Rect spriteCollider = spriteIt.second->GetCollider();                                   \
            if ((currentCollider.x != spriteCollider.x) || (currentCollider.y != spriteCollider.y) ||   \
                (currentCollider.w != spriteCollider.w) || (currentCollider.h != spriteCollider.h)){    \
                    std::cerr << spriteIt.first << " and " << m_spriteState << " do not have matching colliders" << std::endl;  \
                    std::cerr << "    " << "[" << x << "," << y <<"] " << spriteIt.first << " (" << spriteCollider.x << "," << spriteCollider.y << ") (" << spriteCollider.w << "x" << spriteCollider.h << ")" << std::endl;\
                    std::cerr << "    " <<  "[" << currX << "," << currY <<"] " <<  m_spriteState << " (" << currentCollider.x << "," << currentCollider.y << ") (" << currentCollider.w << "x" << currentCollider.h << ")" << std::endl;\
                    std::raise(SIGINT);                                                                 \
            }                                                                                           \
        }                                                                                               \
    }
#else
    #define CHECK_COLLIDERS
#endif // DEBUG_COLLIDERS

#if defined(DEBUG_SPRITE_LOCATIONS)
    #define CHECK_SPRITE_LOCATIONS {                        \
        for (auto & spriteIt : m_sprites) {                 \
            int x =0;int y=0;                               \
            spriteIt.second->GetPosition(x,y);              \
            int currX=0;int currY=0;                        \
            GetCurrentSprite()->GetPosition(currX,currY);   \
            if ((x != currX) || (y != currY)) {             \
                std::raise(SIGINT);                         \
            }                                               \
        }                                                   \
    }
#else
    #define CHECK_SPRITE_LOCATIONS
#endif // DEBUG_SPRITE_LOCATIONS


#define __CLASS__ "PlatformerToken"

#include "Debug.h"


namespace bpl::game {
    PlatformerToken::PlatformerToken() = default;

    PlatformerToken::~PlatformerToken() = default;

    graphics::SpritePtr PlatformerToken::GetSprite(const std::string &state) {
        if (!m_sprites.contains(state)) {
            ERROR_MSG("Called with invalid state(" << state << ")");;

            return nullptr;
        }
        return m_sprites[state];
    } // GetSprite

    int PlatformerToken::GetHealth() const {
        return m_health;
    } // GetHealth

    int PlatformerToken::GetMaxHealth() const {
        return m_maxHealth;
    } // GetMaxHealth

    void PlatformerToken::SetMaxHealth(int maxHealth) {
        m_maxHealth = maxHealth;
    } // SetMaxHealth

    void PlatformerToken::AdjustHealth(int healthChange) {
        if (m_health <= -1 * healthChange) {
            SetState("dead");
        }

        m_health += healthChange;

    } // AdjustHealth

    void PlatformerToken::SetHealth(int health) {
        if (health <= 0) {
            SetState("dead");
        }

        m_health = health;

    } // SetHealth

    graphics::SpritePtr PlatformerToken::GetCurrentSprite() {
        return m_sprites[m_spriteState];
    } // GetCurrentSprite

    void PlatformerToken::SetHeight(int height) {
        for (auto& sprite : std::views::values(m_sprites)) {
            sprite->ScaleToHeight(height);
        }
    } // SetHeight

    void PlatformerToken::SetBounds(const SDL_Rect &bounds) {
        m_bounds = bounds;

        UpdateSpeed_();
    } // SetBounds

    bool PlatformerToken::LoadState(const std::string& state, const std::string& filename) {
        DEBUG_MSG("Loading State: " << state << ":" << filename);

        m_sprites[state] = std::make_shared<bpl::graphics::Sprite>(game::GameStateMachine::getInstance()->GetRenderer(), game::GameStateMachine::getInstance()->GetFramerate());

        if (nullptr == m_sprites[state]) {
            std::cerr << "Failed to load sprite: " << filename << std::endl;
            return false;
        }

        m_sprites[state]->Load(filename);
        m_sprites[state]->setPosition(m_pos.x,m_pos.y);
        m_sprites[state]->ScaleToHeight(m_tokenHeight);

        if (0 == m_spriteState.length()) {
            m_spriteState = state;
        }

        return true;
    } // LoadState

    bool PlatformerToken::LoadState(const std::map<std::string, std::string>& stateMap) {
        for (const auto& it : stateMap) {
            if (!LoadState(it.first, it.second)) {
                return false;
            }
        }

        //
        //  Validate sprites to make sure that we will not have problems with the sprites
        //
        for (const auto& it: m_sprites) {
            for (const auto& innerIt: m_sprites) {
                if (innerIt.second->getWidth() != it.second->getWidth()) {
                    ERROR_MSG("Sprite width mismatch: " << it.first << " != " << innerIt.first);
                }
                if (innerIt.second->getHeight() != it.second->getHeight()) {
                    ERROR_MSG("Sprite height mismatch: " << it.first << " != " << innerIt.first);
                }
            }
        }

        UpdateSpeed_();
        return true;
    } // LoadState

    bool PlatformerToken::SetFloors(const graphics::SpriteList& floors) {
        m_floors = floors;

        return true;
    } // SetFloors

    bool PlatformerToken::SetLadders(const graphics::SpriteList& ladders) {
        m_ladders = ladders;

        return true;
    } // SetLadders

    const std::string& PlatformerToken::GetState() {
        return m_spriteState;
    } // GetState

    bool PlatformerToken::SetState(const std::string& state) {
        if (m_health <= 0) {
            // we cannot change our state if we are dead.
            return false;
        }

        if (!m_sprites.contains(state)) {
            ERROR_MSG("State " << state << "does not exist");

            return false;
        }

        m_spriteState = state;

        return true;
    } // SetState

    void PlatformerToken::UpdateSpeed_() {
        float fps = static_cast<float>(game::GameStateMachine::getInstance()->GetFramerate());

        //
        //  Walking speed rate is the number of seconds it should take to walk across the game area (left to right)
        //
        //
        float rate = static_cast<float>(m_walkingSpeedRate);
        float totalFrames = fps * rate;
        float pixelsPerFrame = static_cast<float>(m_bounds.w) / totalFrames;

        m_walkingSpeed = static_cast<int>(pixelsPerFrame);

        rate = static_cast<float>(m_walkingSpeedRate);
        totalFrames = fps * rate;
        pixelsPerFrame = static_cast<float>(m_bounds.w) / totalFrames;

        m_runningSpeed = static_cast<int>(pixelsPerFrame);
    } // UpdateSpeed_

    void PlatformerToken::SetRunningRate(float rate) {
        m_runningSpeedRate = rate;

        UpdateSpeed_();
    }

    void PlatformerToken::SetWalkingRate(float rate) {
        m_walkingSpeedRate = rate;

        UpdateSpeed_();
    }

    [[nodiscard]] int PlatformerToken::FallDistance_(int maxFallDistance) const {
        return std::min(m_walkingSpeed, maxFallDistance);
    } // FallDistance_()

    bool PlatformerToken::Move(int x, int y) {
        CHECK_FALL_COLLISIONS
        CHECK_SPRITE_LOCATIONS
        CHECK_COLLIDERS

        SDL_Rect lastPosition = m_pos;

        //
        //  Cannot move if we are dead
        //
        if (m_spriteState == "dead") {
            CHECK_FALL_COLLISIONS
            CHECK_SPRITE_LOCATIONS
            CHECK_COLLIDERS

            return false;
        }

        //
        //  Do our movement.
        //
        HandleMovement_(x,y);

        CHECK_FALL_COLLISIONS
        CHECK_SPRITE_LOCATIONS
        CHECK_COLLIDERS

        // If we've had no movement we switch to the idel state
        if (lastPosition.x == m_pos.x && lastPosition.y == m_pos.y && m_spriteState != "dead") {

            CHECK_FALL_COLLISIONS
            CHECK_SPRITE_LOCATIONS
            CHECK_COLLIDERS

            SetState("idle");

            CHECK_FALL_COLLISIONS
            CHECK_SPRITE_LOCATIONS
            CHECK_COLLIDERS
            return false;
        }

        CHECK_FALL_COLLISIONS
        CHECK_SPRITE_LOCATIONS
        CHECK_COLLIDERS

        return true;
    } // Move

    void PlatformerToken::UpdateSpritePosition_() {
        CHECK_FALL_COLLISIONS

        for (auto& it : std::views::values(m_sprites)) {
            it->setPosition(m_pos.x,m_pos.y);
        }

        CHECK_FALL_COLLISIONS
    } // UpdateSpritePosition_

    void PlatformerToken::SetPosition(int x, int y) {
        CHECK_FALL_COLLISIONS
        CHECK_SPRITE_LOCATIONS
        CHECK_COLLIDERS

        m_pos.x = x;
        m_pos.y = y;

        UpdateSpritePosition_();
        CHECK_FALL_COLLISIONS
        CHECK_SPRITE_LOCATIONS
        CHECK_COLLIDERS
    } // SetPosition

    void PlatformerToken::SetStartPosition(int x, int y) {
        m_startPos.x = x;
        m_startPos.y = y;
    } //SetStartPosition

    void PlatformerToken::GotoStartPosition() {
        SetPosition(m_startPos.x, m_startPos.y);
    } // GotoStartPosition


    bool PlatformerToken::HandleBounds_() {
        CHECK_FALL_COLLISIONS

        int imageWidth = m_sprites[m_spriteState]->getWidth();
        int imageHeight = m_sprites[m_spriteState]->getHeight();

        //
        //  Very simple bounds check.
        //
        if (m_pos.x < m_bounds.x) {
            m_pos.x = m_bounds.x;
        }
        if (m_pos.y < m_bounds.y) {
            m_pos.y = m_bounds.y;
        }
        if ((m_pos.y + imageHeight) > (m_bounds.y + m_bounds.h)) {
            m_pos.y = (m_bounds.y + m_bounds.h) - imageHeight;
        }
        if ((m_pos.x + imageWidth) > (m_bounds.x + m_bounds.w)) {
            m_pos.x = (m_bounds.x + m_bounds.w) - imageWidth;
        }

        UpdateSpritePosition_();

        CHECK_FALL_COLLISIONS

        return true;
    } // HandleBounds_

    bool PlatformerToken::HandleFall_(int x, int y) {
        CHECK_FALL_COLLISIONS

        // We will ignore x and y movement requests if we are falling as you cannot change direction while falling

        //  Calculate the maximum falling distance using the current sprites colliders
        //  we check both ladders and floors
        int maxFallDistance = bpl::graphics::collider::Floor::DistanceToFloorBelow(m_floors, GetCurrentSprite()->GetCollider());

        maxFallDistance = std::min(maxFallDistance, bpl::graphics::collider::Floor::DistanceToFloorBelow(m_ladders, GetCurrentSprite()->GetCollider()));

        int fallDistance = FallDistance_(maxFallDistance);

        //
        //  If we are falling, we override any movement requests.
        //
        if ((fallDistance > 0) && (fallDistance != INT_MAX)) {
            m_pos.y += fallDistance;

            SetState("fall");

            UpdateSpritePosition_();

            CHECK_FALL_COLLISIONS

            return true;
        }

        return false;
    } // HandleFall_

    bool PlatformerToken::HandleFloor_(int x, int y) {
        CHECK_FALL_COLLISIONS

        if (x < 0) {
            int distanceLeft = bpl::graphics::collider::Floor::DistanceToWallLeft(m_floors, GetCurrentSprite()->GetCollider());

            m_pos.x -= std::min(distanceLeft, std::abs(x * m_walkingSpeed));

            UpdateSpritePosition_();
            CHECK_FALL_COLLISIONS
            return true;
        }
        else if (x > 0) {
            int distanceRight = bpl::graphics::collider::Floor::DistanceToWallRight(m_floors, GetCurrentSprite()->GetCollider());

            m_pos.x += std::min(distanceRight, std::abs(x * m_walkingSpeed));

            UpdateSpritePosition_();
            CHECK_FALL_COLLISIONS
            return true;
        }

        CHECK_FALL_COLLISIONS

        return false;
    } // HandleFloor_

    bool PlatformerToken::HandleLadder_(int x, int y) {
        CHECK_FALL_COLLISIONS
        //
        //  WARNING: We only use the single collider for ladder checks.  Using multiple colliders is more difficult to
        //           check as we can have one collider in and another out when we should still allow movement.  The
        //           additional accuracy is not likely to gain us anything.
        //
        if (y < 0) {
            for (auto& it: m_ladders) {
                if (bpl::graphics::collider::Ladder::CanClimbUpLadder(it->GetCollider(), m_sprites[m_spriteState]->GetCollider())) {
                    // We can move on a ladder, so we do our movement.
                    m_pos.y += y * m_walkingSpeed;

                    int imageHeight = m_sprites[m_spriteState]->getHeight();
                    SDL_Rect objCollider = m_sprites[m_spriteState]->GetCollider();
                    int imageX = 0;
                    int imageY = 0;
                    m_sprites[m_spriteState]->getPosition(imageX, imageY);

                    int topOfLadderY = it->GetCollider().y - imageHeight + (imageHeight - objCollider.h - (objCollider.y - imageY));

                    if (m_pos.y < topOfLadderY) {
                        m_pos.y = topOfLadderY;
                    }

                    SetState("walk-up");
                    UpdateSpritePosition_();

                    CHECK_FALL_COLLISIONS

                    return true;
                }
            }
        } else if (y > 0) {

            int maxClimbDownDistance = bpl::graphics::collider::Floor::DistanceToFloorBelow(m_floors, GetCurrentSprite()->GetCollider());

            for (auto& it: m_ladders) {
                if (bpl::graphics::collider::Ladder::CanClimbDownLadder(it->GetCollider(), m_sprites[m_spriteState]->GetCollider())) {
                    // We can move on a ladder, so we do our movement.
                    m_pos.y += std::min(y * m_walkingSpeed, maxClimbDownDistance);

                    SetState("walk-down");
                    UpdateSpritePosition_();

                    CHECK_FALL_COLLISIONS
                    return true;
                }
            }
        }

        return false;
    } // HandleLadder_

    void PlatformerToken::HandleMovement_(int x, int y) {
        CHECK_FALL_COLLISIONS

        if (m_health <= 0) {
            m_spriteState = "dead";
            UpdateSpritePosition_();
            return;
        }

        if (m_spriteState == "dead") {
            // We cannot move if we are dead.
            UpdateSpritePosition_();
            return;
        }

        if (HandleFall_(x,y)) {
            // If we are falling, all movement should be completed
            HandleBounds_();

            CHECK_FALL_COLLISIONS

            return;
        }

        // We ignore the handling of the ladder as we will never move in the x direction in our ladder logic.
        bool handledLadder = HandleLadder_(x, y);

        bool handledFloor = HandleFloor_(x, y);

        if ((!handledLadder) && (!handledFloor)) {
            SetState("idle");
        }
        else if (m_spriteState != "walk-up")
        {
            if (x > 0) {
                SetState("walk-right");
            }
            else if (x < 0) {
                SetState("walk-left");
            }
        }
        HandleBounds_();

        UpdateSpritePosition_();

        CHECK_FALL_COLLISIONS
    } // HandleMovement_
}; // bpl::game