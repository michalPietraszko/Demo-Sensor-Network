#pragma once
#include <ISceneModel.hpp>

class SensorCreatorSceneController;

class SensorCreatorSceneModel : public ISceneModel {
    friend class SensorCreatorSceneController;

public:
    enum class SceneStates { start, setStartingValue, acceptSettings, settingsAccepted };

private:
    struct CachedState {
        unsigned numOfSensors{};
    };

    void pullAppInfo(UIAdapter& adapter) override { m_CachedAppState.numOfSensors = adapter.getSensorInfo(); }

    bool wasModelUpdated() override { return false; };

private:
    SceneStates m_CurrentSceneState{SceneStates::start};
    CachedState m_CachedAppState{};
};
