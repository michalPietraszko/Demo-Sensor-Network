#pragma once

#include <ISceneModel.hpp>
#include <ISceneView.hpp>
#include <InputResult.hpp>
#include <SceneChanger.hpp>

class ISceneController {
public:
    ISceneController(ISceneModel& model, ISceneView& view) : m_Model{model}, m_View{view} {}
    virtual ~ISceneController() = default;

    // clang-format off
    void onCreate()                    { onCreateImpl(); }
    void onUpdate(UIAdapter& adapter)  { onUpdateImpl(adapter); } 
    void onLoad(UIAdapter& adapter)    { onLoadImpl(adapter); }
    void onDisplay(Renderer& renderer) { onDisplayImpl(renderer); }
    InputResult onProcessInput()       { return onProcessInputImpl(); }
    // clang-format on

protected:
    virtual void onSceneUpdate() {}
    virtual void onTransition() {}
    virtual bool shouldRenderNextFrame() const { return true; }

protected:
    virtual void onCreateImpl() { m_View.setInvariants(); }

    virtual void onUpdateImpl(UIAdapter& adapter) {
        m_Model.pullAppInfo(adapter);

        if (m_Model.wasModelUpdated()) {
            onSceneUpdate();
        }
    }

    virtual void onLoadImpl(UIAdapter& adapter) {
        m_Model.pullAppInfo(adapter);
        onSceneUpdate();
        m_View.onLoad();
    }

    virtual void onDisplayImpl(Renderer& renderer) {
        if (not renderer.usesOptimizedRendering() or shouldRenderNextFrame()) {
            renderer.newFrame();
            m_View.displayAll(renderer);
        }
        renderer.flush();
    }

    virtual InputResult onProcessInputImpl() { return InputResult::none; }

    template <typename T>
    void setTransition() {
        SceneChanger::setNext<T>();
        onTransition();
    }

protected:
    ISceneModel& m_Model;
    ISceneView& m_View;
};