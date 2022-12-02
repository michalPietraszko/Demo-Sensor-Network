#pragma once
#include <ISceneModel.hpp>

class MainMenuSceneController;

class MainMenuSceneModel : public ISceneModel {
    friend class MainMenuSceneController;

public:
    enum class SceneStates { start, systemReady, systemRunning, invalid, createSensor, createMainNode };

    struct CachedAppState {
        unsigned numOfSensors{0};
        bool isMainNodeCreated{false};
        bool isSystemReady{false};
        bool isSystemRunning{false};

        bool operator==(const CachedAppState& rhs) const {
            auto trick = [](const CachedAppState& state) {
                return std::make_tuple(
                    state.numOfSensors, state.isMainNodeCreated, state.isSystemReady, state.isSystemRunning);
            };

            return trick(*this) == trick(rhs);
        }

        bool operator!=(const CachedAppState& rhs) const { return !(*this == rhs); }
    };

private:
    void pullAppInfo(UIAdapter& adapter) override {
        m_PreUpdateAppState = m_CachedAppState;
        m_CachedAppState.numOfSensors = adapter.getSensorInfo();
        m_CachedAppState.isSystemReady = adapter.isAllSystemSetup();
        m_CachedAppState.isSystemRunning = adapter.isSystemRunning();
        m_CachedAppState.isMainNodeCreated = adapter.isMainNodeCreated();
    }

    bool wasModelUpdated() override { return m_PreUpdateAppState != m_CachedAppState; }

    SceneStates m_CurrentSceneState{SceneStates::start};

    CachedAppState m_CachedAppState{};
    CachedAppState m_PreUpdateAppState{};
};
