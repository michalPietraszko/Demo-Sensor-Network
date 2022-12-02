#pragma once

#include <MainNodeCreatorSceneController.hpp>
#include <MainNodeCreatorSceneModel.hpp>
#include <MainNodeCreatorSceneView.hpp>

class MainNodeCreatorScene : public UIScene {
public:
    MainNodeCreatorScene() { controller.onCreate(); }

    virtual InputResult processUserInput() override { return controller.onProcessInput(); }

    virtual void update(UIAdapter& adapter) override { controller.onUpdate(adapter); }

    virtual void load(UIAdapter& adapter) override { controller.onLoad(adapter); }

    virtual void display(Renderer& renderer) override { controller.onDisplay(renderer); }

private:
    MainNodeCreatorSceneModel model;
    MainNodeCreatorSceneView view;
    MainNodeCreatorSceneController controller{model, view};
};