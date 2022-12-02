#pragma once
#include <Renderer.hpp>

class ISceneView {
public:
    virtual ~ISceneView() = default;
    virtual void displayAll(Renderer&){};
    virtual void onLoad(){};
    virtual void setInvariants() {}
};