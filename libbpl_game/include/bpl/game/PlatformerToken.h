//
// Created by Bradley Remedios on 11/4/24.
//

#ifndef BPL_GAME_PLATFORMERTOKEN_H
#define BPL_GAME_PLATFORMERTOKEN_H

#include <map>
#include <bpl/graphics/Sprite.h>

namespace bpl::game {
    class PlatformerToken {
    public:
        PlatformerToken();
        virtual ~PlatformerToken();

        [[nodiscard]] int GetHealth() const;
        [[nodiscard]] int GetMaxHealth() const;
        void SetHealth(int health);
        void SetMaxHealth(int maxHealth);
        void AdjustHealth(int healthChange);

        [[nodiscard]] bool LoadState(const std::string& state, const std::string& filename);
        [[nodiscard]] bool LoadState(const std::map<std::string, std::string>& stateMap);

        [[nodiscard]] bpl::graphics::SpritePtr GetSprite(const std::string& state);
        [[nodiscard]] bpl::graphics::SpritePtr GetCurrentSprite();
        void SetHeight(int height);

        bool SetState(const std::string& state);
        [[nodiscard]] const std::string& GetState();

        void SetWalkingRate(float rate);
        void SetRunningRate(float rate);

        //
        //  Movement suggestions
        //
        bool Move(int x, int y);
        void SetPosition(int x, int y);
        void SetStartPosition(int x, int y);
        void GotoStartPosition();

        [[nodiscard]] bool SetFloors(const bpl::graphics::SpriteList& floors);
        [[nodiscard]] bool SetLadders(const bpl::graphics::SpriteList& ladders);
        void SetBounds (const SDL_Rect& bounds);

    protected:
        bpl::graphics::SpriteList                        m_floors;
        bpl::graphics::SpriteList                        m_ladders;

        void UpdateSpeed_();

    private:
        std::map<std::string, graphics::SpritePtr>  m_sprites;
        std::string                                 m_spriteState;
        SDL_Rect                                    m_pos = {0,0, 0, 0};
        SDL_Rect                                    m_startPos = {0, 0, 0, 0};
        SDL_Rect                                    m_bounds = {0,0,0,0};
        int                                         m_tokenHeight=128;
        float                                         m_walkingSpeedRate=4;
        float                                         m_runningSpeedRate=4;
        int                                         m_walkingSpeed=10;
        int                                         m_runningSpeed=20;
        int m_maxHealth=100;
        int m_health = 100;

        // Get falling distance, but cap it out at maxFallDistance
        [[nodiscard]] int FallDistance_(int maxFallDistance) const;

        void UpdateSpritePosition_();

        bool HandleFall_(int x, int y);
        bool HandleFloor_(int x, int y);
        bool HandleLadder_(int x, int y);
        void HandleMovement_(int x, int y);
        bool HandleBounds_();

    }; // class PlatformerToken
}; // bpl::game

#endif //BPL_GAME_PLATFORMERTOKEN_H