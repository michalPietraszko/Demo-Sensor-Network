#pragma once

#include <ConsoleRenderer.hpp>
#include <MainMenuScene.hpp>
#include <MainNodeCreatorScene.hpp>
#include <SensorCreatorScene.hpp>
#include <Renderer.hpp>
#include <SceneChanger.hpp>
#include <UI.hpp>
#include <UIAdapter.hpp>
#include <UIScene.hpp>
#include <UserIO.hpp>
#include <utility>

class TextUI : public UI
{
public:
    TextUI(UIAdapter& adapter) : m_Adapter{adapter} { currentScene->load(m_Adapter); }

    void processUserInput() override
    {
        const auto result = currentScene->processUserInput();
        if (result == InputResult::changeLayer)
        {
            currentScene = SceneChanger::nextScene();
            currentScene->load(m_Adapter);
        }
    }

    void update() override { currentScene->update(m_Adapter); }

    void display() override { currentScene->display(*window); }

    UIAdapter& m_Adapter;
    // clang-format off
    std::unique_ptr<UIScene>  currentScene = std::make_unique<MainMenuScene>();
    std::unique_ptr<Renderer> window       = std::make_unique<Renderer>(
                                                std::make_unique<ConsoleRenderer>());
    // clang-format on
};