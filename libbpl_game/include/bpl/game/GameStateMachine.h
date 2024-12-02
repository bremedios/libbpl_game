//
// Created by Bradley Remedios on 10/27/24.
//

#ifndef BPL_GAME_GAMESTATEMACHINE_H
#define BPL_GAME_GAMESTATEMACHINE_H

#include <string>

#include <bpl/sys/Tick.h>
#include <bpl/graphics/Window.h>
#include <bpl/graphics/Renderer.h>

#include <bpl/game/GameState.h>

namespace bpl::game {
    class GameStateMachine {
    public:
        static GameStateMachine* getInstance();

        GameStateMachine();
        virtual ~GameStateMachine()=default;

        [[nodiscard]] bool Create(const std::string& title, int width=1920, int height=1080, int framerate=30, bool fullscreen=false);
        void Shutdown();

        void GetWindowSize(int& width, int& height) const;

        //
        //  Object retrieval routines.
        //
        [[nodiscard]] bpl::graphics::RendererPtr& GetRenderer();
        [[nodiscard]] int GetFramerate() const;

        //
        //  This starts the main game.
        //
        void Run();
        void SetNextState(bpl::game::GameState *nextState);
        void Terminate();

    private:
        //
        //  Graphics objects
        //
        bpl::sys::Tick                 m_tick;
        bpl::graphics::WindowPtr       m_window   = nullptr;
        bpl::graphics::RendererPtr     m_renderer = nullptr;

        //
        //  State Data
        //
        bpl::game::GameState*    m_currentState  = nullptr;
        bpl::game::GameState*    m_nextState     = nullptr;
        int                      m_windowWidth   = 0;
        int                      m_windowHeight  = 0;
        bool                     m_terminate     = false;
        int                      m_framerate     = 30;

    };
    typedef std::shared_ptr<GameStateMachine> GameStateMachinePtr;

} // bpl::game

#endif //BPL_GAME_GAMESTATEMACHINE_H