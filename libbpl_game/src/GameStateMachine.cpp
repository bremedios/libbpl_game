//
// Created by Bradley Remedios on 10/27/24.
//

#include <chrono>
#include <iostream>

#include <bpl/sys/Stopwatch.h>

#include <bpl/controls/Input.h>

#include <bpl/game/GameStateMachine.h>

#include "Debug.h"

namespace bpl::game {
    GameStateMachine::GameStateMachine()
        : m_tick(std::chrono::milliseconds(1000/30)) {
    }

    GameStateMachine *GameStateMachine::getInstance() {
        static GameStateMachine instance;

        return &instance;
    } // getInstance

    graphics::RendererPtr& GameStateMachine::GetRenderer() {
        return m_renderer;
    } // GetRenderer

    bool GameStateMachine::Create(const std::string& title, int width, int height, int framerate, bool fullscreen) {
        m_framerate = framerate;
        //
        //  Allocate required graphics objects
        //
        m_window = std::make_shared<bpl::graphics::Window>();

        if (nullptr == m_window) {
            std::cerr << "Failed to allocate window" << std::endl;
            return false;
        }

        m_renderer = std::make_shared<bpl::graphics::Renderer>();

        if (nullptr == m_renderer) {
            std::cerr << "Failed to allocate renderer" << std::endl;
            return false;
        }

        m_windowWidth = width;
        m_windowHeight = height;

        m_tick.setPeriod(1000/framerate);

        if (!m_window->Create(title, width, height, fullscreen)) {
            std::cerr << "Failed to create window." << std::endl;
            return false;
        }

        if (!m_renderer->Create(m_window)) {
            std::cerr << "Failed to create renderer." << std::endl;

            return false;
        }

        return true;
    } // Create

    void GameStateMachine::GetWindowSize(int& width, int& height) const {
        width = m_windowWidth;
        height = m_windowHeight;
    } // GetWindowSize

    int GameStateMachine::GetFramerate() const {
        return m_framerate;
    } // GetFramerate

    void GameStateMachine::Shutdown() {
        //
        //  Window has no destroy
        //
        m_window = nullptr;

        if (nullptr != m_renderer) {
            m_renderer->Destroy();
            m_renderer = nullptr;
        }
    } // Shutdown

    void GameStateMachine::Terminate() {
        m_terminate = true;
    } // Terminate

    void GameStateMachine::Run() {
        SDL_Event evt;

        if (nullptr == m_nextState) {
            std::cerr << "Game state machine not setup." << std::endl;
            return;
        }

        m_currentState = m_nextState;
        m_nextState = nullptr;
        if (!m_currentState->Entry()) {
            std::cerr << "Failed to initialize game state, terminating" << std::endl;
            return;
        }

        while (!m_terminate) {
            controls::Input::getInstance()->Update();

            //
            //  Event Handling
            //
            while (SDL_PollEvent(&evt)) {
                controls::Input::getInstance()->Process(evt);

                m_currentState->Event(evt);

                if (evt.type == SDL_QUIT) {
                    m_terminate = true;
                }
            }

            //
            //  Logic Handling
            //
            m_currentState->Update();

            //
            //  Rendering
            //
            m_currentState->RenderStart(m_renderer);
            m_currentState->Render(m_renderer);
            m_currentState->RenderEnd(m_renderer);

            //
            //  This will handle keeping our framerate stable and correct.
            //
            m_tick.Wait();

            //
            //  Switch States if we have it setup
            //
            if (nullptr != m_nextState) {
                m_currentState->Exit();
                m_currentState = m_nextState;
                m_nextState = nullptr;
                bpl::sys::Stopwatch sw;
                sw.Start();
                if (!m_currentState->Entry()) {
                    std::cerr << "Failed to initialize game state.  Terminating." << std::endl;
                    return;
                }
                sw.Stop();

                std::cout << "Entry Time: " << sw.getMilliseconds() << std::endl;
            }
        }
    } // Run

    void GameStateMachine::SetNextState(class GameState *nextState) {
        m_nextState = nextState;
    } // SetNextState
} // bpl::game