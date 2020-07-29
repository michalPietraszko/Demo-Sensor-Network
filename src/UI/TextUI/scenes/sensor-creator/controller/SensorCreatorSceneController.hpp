#pragma once

#include <ISceneController.hpp>
#include <SensorCreatorSceneModel.hpp>
#include <SensorCreatorSceneView.hpp>

#include <InputParser.hpp>
#include <InputProcessor.hpp>
#include <SceneChanger.hpp>
#include <SceneLifetimeInfo.hpp>

#include <InputResult.hpp>
#include <LazyInvoke.hpp>

class MainMenuScene;
class SensorCreatorSceneController : public ISceneController
{
    // clang-format off
    using Model = SensorCreatorSceneModel;
    using View  = SensorCreatorSceneView;

    using SceneStates   = Model::SceneStates;
    using TextConstants = View::TextConstants;
    using ViewElementId = View::ViewElementId;

    using AdapterCaller       = LazyInvoke<decltype(&UIAdapter::setupSensor)>;
    using SceneInputParser    = InputParser<SceneStates, SceneStates>;
    using input_process_fn_t  = std::function<void(void)>;
    using SceneInputProcessor = InputProcessor<input_process_fn_t>;
    // clang-format on

public:
    SensorCreatorSceneController(Model& model, View& view) : ISceneController(model, view) {}

protected:
    void onCreateImpl() override
    {
        ISceneController::onCreateImpl();
        setInputCallbacks();
        setSensorName();
    }

    InputResult onProcessInputImpl() override
    {
        inputProcessor.processAllInput();
        return getSceneLifetimeFromCurrentState();
    }

    void onLoadImpl(UIAdapter& adapter) override
    {
        ISceneController::onLoadImpl(adapter);
        adapterCache = &adapter;
    }

    void onDisplayImpl(Renderer& renderer) override
    {
        ISceneController::onDisplayImpl(renderer);
        isNewestStateRendered = true;
    }

    void onTransition() override { lifetimeInfo.setSceneChangeFlag(); }

    void onSceneUpdate() override { isNewestStateRendered = false; }

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
            auto& label = getView().m_IO.appStatus.get();
            label.setTextCallback([]() { return " Invalid input!"; });
            return getModel().m_CurrentSceneState;
        };

        inputParser.enable(SceneStates::start, invalidInputCallback);
        inputParser.enable(SceneStates::setStartingValue, invalidInputCallback);
        inputParser.enable(SceneStates::acceptSettings, invalidInputCallback);

        using Creator = InputMatcherCreator;

        inputParser.addCallback(SceneStates::start, {Creator::numberInRange(500, 5000)}, [this](const auto& input) {
            onSetStartingValueTransition(input);
            return SceneStates::setStartingValue;
        });

        inputParser.addCallback(
            SceneStates::setStartingValue, {Creator::numberInRange(1, 1000)}, [this](const auto& input) {
                onAcceptSettingsTransition(input);
                return SceneStates::acceptSettings;
            });

        inputParser.addCallback(
            SceneStates::acceptSettings, {Creator::stringEqual("yes"), Creator::stringEqual("y")}, [this](const auto&) {
                setTransition<MainMenuScene>();
                creatorProcedure.invoke(*adapterCache);
                return SceneStates::settingsAccepted;
            });

        inputParser.addCallback(
            SceneStates::acceptSettings, {Creator::stringEqual("no"), Creator::stringEqual("n")}, [this](const auto&) {
                updateLabel(SceneStates::start);
                getView().onLoad();
                getView().removeMenuPos(ViewElementId::startingValue);
                return SceneStates::start;
            });
    }

    void setSensorName()
    {
        getMenuCell(ViewElementId::sensorName).status.setText(TextConstants::sensorName);
        getMenuCell(ViewElementId::sensorName).status.setTextCallback([this]() {
            return std::to_string(getModel().m_CachedAppState.numOfSensors);
        });
    }

    void onNewIOUserInput(const UITextField& userInput)
    {
        getModel().m_CurrentSceneState = inputParser.parse(getModel().m_CurrentSceneState, userInput.getText());
        isNewestStateRendered = false;
    }

    void updateLabel(const SceneStates newState)
    {
        auto& label = getView().m_IO.appStatus.get();
        label.resetTextCallback();

        switch (newState)
        {
            case SceneStates::setStartingValue:
                label.setText(TextConstants::labelStartingVal);
                break;
            case SceneStates::acceptSettings:
                label.setText(TextConstants::labelAccept);
                break;
            case SceneStates::settingsAccepted:
            case SceneStates::start:
                break;
        }
    }

    void onSetStartingValueTransition(const std::any& input)
    {
        updateLabel(SceneStates::setStartingValue);
        creatorProcedure.clear();

        auto sensorName = getMenuCell(ViewElementId::sensorName).status.getText();
        creatorProcedure.addArgument(sensorName);

        const auto intInput = std::any_cast<int>(input);
        getMenuCell(ViewElementId::interval)
            .status.setText(TextConstants::enterIntervalStatus + std::to_string(intInput));

        getView().addMenuPos(ViewElementId::startingValue);
        getMenuCell(ViewElementId::startingValue).description.setText(TextConstants::enterStartingValStr);

        creatorProcedure.addArgument(intInput);
    }

    void onAcceptSettingsTransition(const std::any& input)
    {
        updateLabel(SceneStates::acceptSettings);

        const auto intInput = std::any_cast<int>(input);
        getMenuCell(ViewElementId::startingValue)
            .status.setText(TextConstants::enterStartingValStatus + std::to_string(intInput));

        creatorProcedure.addArgument(intInput);
    }

    UIStatusCell& getMenuCell(const ViewElementId& id) { return getView().view.menu.at(id); }

    InputResult getSceneLifetimeFromCurrentState()
    {
        return lifetimeInfo.shouldChangeScene() ? InputResult::changeLayer : InputResult::none;
    }

    View& getView() { return dynamic_cast<View&>(m_View); }

    Model& getModel() { return dynamic_cast<Model&>(m_Model); }

    SceneInputProcessor inputProcessor{};
    SceneLifetimeInfo lifetimeInfo{};
    SceneInputParser inputParser{};
    AdapterCaller creatorProcedure{&UIAdapter::setupSensor};
    UIAdapter* adapterCache{nullptr};

    bool isNewestStateRendered{false};
};
