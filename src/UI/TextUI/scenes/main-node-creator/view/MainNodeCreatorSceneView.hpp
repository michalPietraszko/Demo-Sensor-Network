#pragma once
#include <ISceneView.hpp>
#include <MainNodeCreatorSceneModel.hpp>
#include <MenuView.hpp>
#include <UserIO.hpp>

class MainNodeCreatorSceneController;

class MainNodeCreatorSceneView : public ISceneView
{
    struct TextConstants
    {
        constexpr static auto headline = "Create Main Node";
        constexpr static auto set_bsize_name = "Set circular-buffer size: ";
        constexpr static auto set_bsize_status = "Buffer size: ";
        constexpr static auto empty = "";

        constexpr static auto label_set_bsize = "Enter positive integer [1...10]";
        constexpr static auto label_prompt_accept = "Accept settings? [y/n]";
    };

    enum class ViewElementId
    {
        setBufferSize
    };

    friend class MainNodeCreatorSceneController;
    using SceneStates = MainNodeCreatorSceneModel::SceneStates;
    using MainView = MenuView<ViewElementId>;

    void setInvariants() override
    {
        view.menu.at(ViewElementId::setBufferSize).description.setText(TextConstants::set_bsize_name);
    }

    void displayAll(Renderer& renderer) override
    {
        view.display(renderer);
        m_IO.appStatus.display(renderer);
        m_IO.userInput.display(renderer);
    }

    void onLoad() override
    {
        view.menu.at(ViewElementId::setBufferSize).status.setText(TextConstants::empty);
        m_IO.appStatus.get().setText(TextConstants::label_set_bsize);
    }

// clang-format off
    MainView view = {
        UITextField{
            TextConstants::headline}, 
        MainView::MappedMenu{{
            MainView::createMenuEntry(ViewElementId::setBufferSize)}}
    };

    UserIO m_IO{};
// clang-format on
};