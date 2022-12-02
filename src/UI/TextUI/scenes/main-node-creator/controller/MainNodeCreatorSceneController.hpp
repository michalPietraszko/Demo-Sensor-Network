#pragma once

#include <ISceneController.hpp>
#include <MainNodeCreatorSceneModel.hpp>
#include <MainNodeCreatorSceneView.hpp>

#include <InputParser.hpp>
#include <InputProcessor.hpp>
#include <SceneChanger.hpp>
#include <SceneLifetimeInfo.hpp>

#include <InputResult.hpp>
#include <LazyInvoke.hpp>

class MainMenuScene;
class MainNodeCreatorSceneController : public ISceneController {
    // clang-format off
    using Model = MainNodeCreatorSceneModel;
    using View  = MainNodeCreatorSceneView;

    using SceneStates   = Model::SceneStates;
    using TextConstants = View::TextConstants;
    using ViewElementId = View::ViewElementId;

    using AdapterCaller       = LazyInvoke<decltype(&UIAdapter::setupMainNode)>;
    using SceneInputParser    = InputParser<SceneStates, SceneStates>;
    using input_process_fn_t  = std::function<void(void)>;
    using SceneInputProcessor = InputProcessor<input_process_fn_t>;
    // clang-format on

public:
    MainNodeCreatorSceneController(Model& model, View& view) : ISceneController(model, view) {}

protected:
    void onCreateImpl() override {
        ISceneController::onCreateImpl();
        setInputCallbacks();
    }

    InputResult onProcessInputImpl() override {
        inputProcessor.processAllInput();
        return getSceneLifetimeFromCurrentState();
    }

    void onLoadImpl(UIAdapter& adapter) override {
        ISceneController::onLoadImpl(adapter);
        adapterCache = &adapter;
    }

    void onDisplayImpl(Renderer& renderer) override {
        ISceneController::onDisplayImpl(renderer);
        isNewestStateRendered = true;
    }

    void onTransition() override { lifetimeInfo.setSceneChangeFlag(); }

    void onSceneUpdate() override { isNewestStateRendered = false; }

    bool shouldRenderNextFrame() const override { return not isNewestStateRendered; }

private:
    void setInputCallbacks() {
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
        inputParser.enable(SceneStates::acceptSettings, invalidInputCallback);

        using Creator = InputMatcherCreator;

        inputParser.addCallback(SceneStates::start, {Creator::numberInRange(1, 10)}, [this](const auto& input) {
            creatorProcedure.clear();

            auto& label = getView().m_IO.appStatus.get();
            label.resetTextCallback();
            label.setText(TextConstants::label_prompt_accept);

            const auto intInput = std::any_cast<int>(input);
            auto& status = getView().view.menu.at(ViewElementId::setBufferSize).status;
            status.setText(TextConstants::set_bsize_status + std::to_string(intInput));

            creatorProcedure.addArgument(intInput);
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
                auto& label = getView().m_IO.appStatus.get();
                label.resetTextCallback();
                getView().onLoad();
                return SceneStates::start;
            });
    }

    void onNewIOUserInput(const UITextField& userInput) {
        getModel().m_CurrentSceneState = inputParser.parse(getModel().m_CurrentSceneState, userInput.getText());
        isNewestStateRendered = false;
    }

    InputResult getSceneLifetimeFromCurrentState() {
        return lifetimeInfo.shouldChangeScene() ? InputResult::changeLayer : InputResult::none;
    }

    View& getView() { return dynamic_cast<View&>(m_View); }

    Model& getModel() { return dynamic_cast<Model&>(m_Model); }

    SceneInputProcessor inputProcessor{};
    SceneLifetimeInfo lifetimeInfo{};
    SceneInputParser inputParser{};
    AdapterCaller creatorProcedure{&UIAdapter::setupMainNode};
    UIAdapter* adapterCache{nullptr};

    bool isNewestStateRendered{false};
};