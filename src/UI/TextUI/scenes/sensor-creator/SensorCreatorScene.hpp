#pragma once

#include <SensorCreatorSceneView.hpp>
#include <SensorCreatorSceneModel.hpp>
#include <SensorCreatorSceneController.hpp>

class SensorCreatorScene : public UIScene
{
public:
    SensorCreatorScene() { controller.onCreate(); }

    virtual InputResult processUserInput()   override { return controller.onProcessInput(); }

    virtual void update(UIAdapter& adapter)  override { controller.onUpdate(adapter); }

    virtual void load(UIAdapter& adapter)    override { controller.onLoad(adapter); }

    virtual void display(Renderer& renderer) override { controller.onDisplay(renderer); }

private:
    SensorCreatorSceneModel model;
    SensorCreatorSceneView view;
    SensorCreatorSceneController controller{model, view};
};
