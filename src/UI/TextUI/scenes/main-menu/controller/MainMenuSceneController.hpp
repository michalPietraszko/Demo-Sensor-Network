#pragma once

#include <ISceneController.hpp>
#include <MainMenuSceneModel.hpp>
#include <MainMenuSceneView.hpp>

#include <InputParser.hpp>
#include <InputProcessor.hpp>
#include <SceneChanger.hpp>
#include <SceneLifetimeInfo.hpp>

#include <InputResult.hpp>

class MainNodeCreatorScene;
class SensorCreatorScene;
class MainMenuSceneController : public ISceneController
{
    // clang-format off
    using Model = MainMenuSceneModel;
    using View  = MainMenuSceneView;

    using SceneStates   = Model::SceneStates;
    using TextConstants = View::TextConstants;

    using SceneInputParser    = InputParser<SceneStates, SceneStates>;
    using input_process_fn_t  = std::function<void(void)>;
    using SceneInputProcessor = InputProcessor<input_process_fn_t>;
    // clang-format on

public:
    MainMenuSceneController(Model& model, View& view) : ISceneController(model, view) {}

protected:
    void onCreateImpl() override
    {
        ISceneController::onCreateImpl();
        setInputCallbacks();
        addNumOfSensorsToSensorStatus();
    }

    InputResult onProcessInputImpl() override
    {
        inputProcessor.processAllInput();
        return getSceneLifetimeFromCurrentState();
    }

    void onLoadImpl(UIAdapter& adapter) override /* temp */
    {
        ISceneController::onLoadImpl(adapter);
        adapterCache = &adapter;
        updateBasedOnStartingState();
    }

    void onDisplayImpl(Renderer& renderer) override
    {
        ISceneController::onDisplayImpl(renderer);
        isNewestStateRendered = true;
    }

    void onTransition() override { lifetimeInfo.setSceneChangeFlag(); }

    void onSceneUpdate() override
    {
        auto& node = getView().view.menu.at(SceneStates::createMainNode).status;
        getModel().m_CachedAppState.isMainNodeCreated ? node.setText(TextConstants::main_node_created)
                                                      : node.setText(TextConstants::main_node_not_created);

        auto& sensor = getView().view.menu.at(SceneStates::createSensor).status;
        getModel().m_CachedAppState.numOfSensors ? sensor.setText(TextConstants::sensor_created)
                                                 : sensor.setText(TextConstants::sensor_not_created);

        isNewestStateRendered = false;
    }

    bool shouldRenderNextFrame() const override { return not isNewestStateRendered; }

private:
    void setInputCallbacks()
    {
        inputProcessor.add([this]() {
            auto in = getView().m_IO.userInput.popInput();
            if (not in) return;

            onNewIOUserInput(*in);
        });

        auto invalidInputCallback = [this]() {
            setLabelCallback(", Invalid input!");
            return getModel().m_CurrentSceneState;
        };

        // clang-format off
        inputParser.enable(SceneStates::start,         invalidInputCallback);
        inputParser.enable(SceneStates::systemReady,   invalidInputCallback);
        inputParser.enable(SceneStates::systemRunning, invalidInputCallback);
        //clang-format on

        inputParser.addCallback(
            {SceneStates::start, SceneStates::systemReady}, {InputMatcherCreator::numberEqual(1)}, [this](const auto&) {
                if (getModel().m_CachedAppState.isMainNodeCreated)
                {
                    setLabelCallback(", Main node already created!");
                    return SceneStates::start;
                }
                setTransition<MainNodeCreatorScene>();
                return SceneStates::createMainNode;
            });

        inputParser.addCallback(
            {SceneStates::systemRunning, SceneStates::systemReady, SceneStates::start},
            {InputMatcherCreator::numberEqual(2)},
            [this](const auto&) {
                if (not getModel().m_CachedAppState.isMainNodeCreated)
                {
                    setLabelCallback(", Create Main Node first!");
                    return SceneStates::start;
                }
                setTransition<SensorCreatorScene>();
                return SceneStates::createSensor;
            });

        inputParser.addCallback(
            SceneStates::systemReady,
            {InputMatcherCreator::stringEqual("run"), InputMatcherCreator::stringEqual("r")},
            [this](const auto&) {
                setLabel("System is running, you can only add sensors now, type \'stop\' to exit");
                adapterCache->startSystem();
                return SceneStates::systemRunning;
            });

        inputParser.addCallback(
            SceneStates::systemRunning,
            {InputMatcherCreator::stringEqual("stop"), InputMatcherCreator::stringEqual("s")},
            [this](const auto&) {
                setLabel("System finished, you can check \'logs\' folder to see the system runtime");
                adapterCache->stopSystem();
                return SceneStates::start;
            });
    }

    void updateBasedOnStartingState()
    {
        auto& model = getModel();
        if (not model.m_CachedAppState.isSystemReady) return;

        if (model.m_CachedAppState.isSystemRunning)
        {
            setLabel("System is running, you can only add sensors now, type \'stop\' to exit");
            model.m_CurrentSceneState = SceneStates::systemRunning;
        }

        else if (model.m_CachedAppState.isSystemReady)
        {
            setLabel("System is ready, add more components or type \'run\' to start the simulation");
            model.m_CurrentSceneState = SceneStates::systemReady;
        }
    }

    void setLabel(const std::string& newText, const std::string& callbackStr = "")
    {
        auto& label = getView().m_IO.appStatus.get();
        label.setText(newText);
        label.setTextCallback([callbackStr]() { return callbackStr; });
    }

    void setLabelCallback(const std::string& callbackStr)
    {
        auto& label = getView().m_IO.appStatus.get();
        label.setTextCallback([callbackStr]() { return callbackStr; });
    }

    InputResult getSceneLifetimeFromCurrentState()
    {
        return lifetimeInfo.shouldChangeScene() ? InputResult::changeLayer : InputResult::none;
    }

    void addNumOfSensorsToSensorStatus()
    {
        getView().view.menu.at(SceneStates::createSensor).status.setTextCallback([this]() {
            return std::string(", ")
                .append(TextConstants::additional_sensor_info)
                .append(std::to_string(getModel().m_CachedAppState.numOfSensors));
        });
    }

    void onNewIOUserInput(const UITextField& userInput)
    {
        getModel().m_CurrentSceneState = inputParser.parse(getModel().m_CurrentSceneState, userInput.getText());
        isNewestStateRendered = false;
    }

    View& getView() { return dynamic_cast<View&>(m_View); }

    Model& getModel() { return dynamic_cast<Model&>(m_Model); }

    SceneInputProcessor inputProcessor{};
    SceneLifetimeInfo lifetimeInfo{};
    SceneInputParser inputParser{};
    UIAdapter* adapterCache{nullptr};

    bool isNewestStateRendered{false};
};
