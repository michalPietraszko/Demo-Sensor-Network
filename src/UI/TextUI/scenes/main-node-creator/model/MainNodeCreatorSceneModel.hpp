#pragma once
#include <ISceneModel.hpp>

class MainNodeCreatorSceneController;

class MainNodeCreatorSceneModel : public ISceneModel
{
    friend class MainNodeCreatorSceneController;
public:
    enum class SceneStates
    {
        start,
        invalid,
        acceptSettings,
        settingsAccepted
    };

    bool wasModelUpdated() override { return false; };

private:
    SceneStates m_CurrentSceneState{SceneStates::start};
};