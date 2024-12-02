//
// Created by Bradley Remedios on 10/27/24.
//

#ifndef BPL_GAME_GAMESTATE_H
#define BPL_GAME_GAMESTATE_H

#include <SDL2/SDL_events.h>

#include <bpl/graphics/RenderObject.h>
#include <bpl/graphics/Renderer.h>

namespace bpl::game {
    class GameState : public bpl::graphics::RenderObject{
    public:
        virtual ~GameState() = default;

        //
        //  Setup and Teardown routines
        //
        [[nodiscard]] virtual bool Entry()=0;
        [[nodiscard]] virtual bool Exit()=0;

        //
        //  Game handling routines
        //
        virtual void Event(SDL_Event& event)=0;
        virtual void Update()=0;
        virtual void RenderStart(bpl::graphics::RendererPtr& renderer)=0;
        virtual void Render(bpl::graphics::RendererPtr& renderer)=0;
        virtual void RenderEnd(bpl::graphics::RendererPtr& renderer)=0;
    protected:
        GameState() = default;

        std::string m_name = "";
    }; // GameState

}; // bpl::game
#endif //BPL_GAME_GAMESTATE_H