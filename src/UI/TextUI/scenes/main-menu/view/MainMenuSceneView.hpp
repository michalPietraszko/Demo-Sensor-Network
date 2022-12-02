#pragma once
#include <ISceneView.hpp>
#include <MainMenuSceneModel.hpp>
#include <MenuView.hpp>
#include <UserIO.hpp>

class MainMenuSceneController;

class MainMenuSceneView : public ISceneView {
    struct TextConstants {
        constexpr static auto headline = "Main Menu";
        constexpr static auto create_main_node = "Create Main-Node";
        constexpr static auto create_sensor = "Create Sensor";
        constexpr static auto additional_sensor_info = "num of Sensors: ";

        constexpr static auto main_node_created = "Main Node created";
        constexpr static auto main_node_not_created = "Main Node not created";

        constexpr static auto sensor_created = "Sensor created";
        constexpr static auto sensor_not_created = "No Sensor created";

        constexpr static auto initial_label = "Choose option by typing \'1\' or \'2\' \'q\' to quit";
    };

    friend class MainMenuSceneController;
    using SceneStates = MainMenuSceneModel::SceneStates;
    using MainView = MenuView<SceneStates>;

    void setInvariants() override {
        view.menu.at(SceneStates::createMainNode).description.setText(TextConstants::create_main_node);
        view.menu.at(SceneStates::createSensor).description.setText(TextConstants::create_sensor);
    }

    void displayAll(Renderer& renderer) override {
        view.display(renderer);
        m_IO.appStatus.display(renderer);
        m_IO.userInput.display(renderer);
    }

    void onLoad() override {
        auto& label = m_IO.appStatus.get();
        label.setText(TextConstants::initial_label);
    }

    // clang-format off
    MainView view = {
        UITextField{
            TextConstants::headline},
        MainView::MappedMenu{{
            MainView::createMenuEntry(SceneStates::createMainNode), 
            MainView::createMenuEntry(SceneStates::createSensor)}}
    };

    UserIO m_IO{};
}; // clang-format on
