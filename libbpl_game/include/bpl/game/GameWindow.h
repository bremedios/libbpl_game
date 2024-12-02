//
// Created by Bradley Remedios on 11/13/24.
//

#ifndef BPL_GAME_GAMEWINDOW_H
#define BPL_GAME_GAMEWINDOW_H

#include <string>
#include <map>

#include <SDL2/SDL.h>

#include <rapidjson/document.h>

#include <bpl/graphics/Sprite.h>
#include <bpl/graphics/ui/Window.h>

#include <bpl/game/GameState.h>

namespace bpl::game {

    class GameWindow  : public bpl::game::GameState {
    public:
        GameWindow(const std::string& layoutFile, const std::string& name);
        ~GameWindow() override;

        //
        //  Setup and Teardown routines
        //
        [[nodiscard]] bool Entry() override;
        [[nodiscard]] bool Exit() override;

        //
        //  Game handling routines
        //
        void Event(SDL_Event& event) override;
        void Update() override;
        void RenderStart(bpl::graphics::RendererPtr& renderer) override;
        void Render(bpl::graphics::RendererPtr& renderer) override;
        void RenderEnd(bpl::graphics::RendererPtr& renderer) override;
    protected:
        GameWindow();

        bpl::graphics::ui::WindowPtr    m_window;
        std::string                     m_name;
        std::string                     m_layoutFile;


    private:
    }; // GameWindow

} // bpl::game

#endif //BPL_GAME_GAMEWINDOW_H