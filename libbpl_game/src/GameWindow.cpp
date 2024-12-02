//
// Created by Bradley Remedios on 11/13/24.
//

#include <iostream>
#include <fstream>
#include <filesystem>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <bpl/game/GameWindow.h>

#include <bpl/game/GameStateMachine.h>

namespace bpl::game {
    GameWindow::GameWindow() {
    }

    GameWindow::GameWindow(const std::string& layoutFile, const std::string& name)
        : m_layoutFile(layoutFile)
        , m_name(name) {

        if (m_layoutFile.empty()) {
            std::cerr << "GameWindow: No layout file specified" << std::endl;
        }
    }

    GameWindow::~GameWindow() {

    }

    bool GameWindow::Entry() {
        int width = 0;
        int height = 0;
        bpl::game::GameStateMachine::getInstance()->GetWindowSize(width, height);

        m_window = std::make_shared<bpl::graphics::ui::Window>(m_layoutFile);

        if (!m_window->Load(bpl::game::GameStateMachine::getInstance()->GetRenderer())) {
            std::cerr << "GameWindow::Entry: Window::Load() failed" << std::endl;
            std::cerr << "    width:  " << width << std::endl;
            std::cerr << "    height: " << height << std::endl;

            return false;
        }

        return true;
    } // Entry

    bool GameWindow::Exit() {
        m_window = nullptr;

        return true;
    } // Exit

    void GameWindow::Event(SDL_Event& event) {

    } // Event

    void GameWindow::Update() {

    } // Update

    void GameWindow::RenderEnd(graphics::RendererPtr& renderer) {
        SDL_RenderPresent(renderer->getSdlRenderer());
    } // RenderEnd

    void GameWindow::RenderStart(graphics::RendererPtr& renderer) {
        SDL_RenderClear(renderer->getSdlRenderer());
    } // RenderStart

    void GameWindow::Render(graphics::RendererPtr& renderer) {
        m_window->Render(renderer);
    } // Render
} // bpl::game