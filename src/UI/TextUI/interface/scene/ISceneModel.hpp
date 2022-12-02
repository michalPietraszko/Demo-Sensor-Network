#pragma once
#include <UIAdapter.hpp>

class ISceneModel {
public:
    virtual ~ISceneModel() = default;

    virtual void pullAppInfo(UIAdapter&) {}
    virtual bool wasModelUpdated() { return false; }
};