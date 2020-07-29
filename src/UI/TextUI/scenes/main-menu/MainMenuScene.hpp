#pragma once

#include <MainMenuSceneView.hpp>
#include <MainMenuSceneModel.hpp>
#include <MainMenuSceneController.hpp>

class MainMenuScene : public UIScene
{
public:
    MainMenuScene() { controller.onCreate(); }

    virtual InputResult processUserInput()   override { return controller.onProcessInput(); }

    virtual void update(UIAdapter& adapter)  override { controller.onUpdate(adapter); }

    virtual void load(UIAdapter& adapter)    override { controller.onLoad(adapter); }

    virtual void display(Renderer& renderer) override { controller.onDisplay(renderer); }

private:
    MainMenuSceneModel model;
    MainMenuSceneView view;
    MainMenuSceneController controller{model, view};
};
