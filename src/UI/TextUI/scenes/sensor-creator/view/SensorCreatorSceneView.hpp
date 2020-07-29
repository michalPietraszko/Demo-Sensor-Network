#pragma once
#include <ISceneView.hpp>
#include <MenuView.hpp>
#include <SensorCreatorSceneModel.hpp>
#include <UserIO.hpp>

class SensorCreatorSceneController;

class SensorCreatorSceneView : public ISceneView
{
    struct TextConstants // clang-format off
    {
        constexpr static auto headline = "Create Sensor";

        constexpr static auto empty = "";
        constexpr static auto sensorName = "Sensor-";

        constexpr static auto sensorNameStr       = "Sensor name: ";
        constexpr static auto enterIntervalStr    = "Set reporting period [ms]: ";
        constexpr static auto enterStartingValStr = "Set message starting value: ";

        constexpr static auto enterIntervalStatus    = "Period: ";
        constexpr static auto enterStartingValStatus = "Value: ";

        constexpr static auto labelInvalid     = "Invalid input!";
        constexpr static auto labelInterval    = "Enter a positive integer [500...5000]";
        constexpr static auto labelStartingVal = "Enter a positive integer [1...1000]";
        constexpr static auto labelAccept      = "Accept settings? [y/n]";
    }; // clang-format on

    enum class ViewElementId
    {
        sensorName,
        interval,
        startingValue
    };

    friend class SensorCreatorSceneController;
    using SceneStates = SensorCreatorSceneModel::SceneStates;
    using MainView = MenuView<ViewElementId>;

    void setInvariants() override
    {
        view.menu.at(ViewElementId::sensorName).description.setText(TextConstants::sensorNameStr);
    }

    void displayAll(Renderer& renderer) override
    {
        view.display(renderer);
        m_IO.appStatus.display(renderer);
        m_IO.userInput.display(renderer);
    }

    void onLoad() override
    {
        auto& label = m_IO.appStatus.get();
        label.resetTextCallback();

        view.menu.at(ViewElementId::interval).description.setText(TextConstants::enterIntervalStr);
        view.menu.at(ViewElementId::interval).status.setText(TextConstants::empty);
        label.setText(TextConstants::labelInterval);
    }

    void addMenuPos(ViewElementId key)
    {
        view.menu.add(key, {});
    }

    void removeMenuPos(ViewElementId key)
    {
        view.menu.remove(key);
    }

    // clang-format off
    MainView view = {
        UITextField{
            TextConstants::headline}, 
        MainView::MappedMenu{{
            MainView::createMenuEntry(ViewElementId::sensorName),
            MainView::createMenuEntry(ViewElementId::interval)
            /*,MainView::createMenuEntry(ViewElementId::startingValue)*/}}
    };

    UserIO m_IO{};
    // clang-format on
};
