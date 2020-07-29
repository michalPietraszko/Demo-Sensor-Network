#pragma once

#include <IFrame.hpp>

class IRenderingMethod
{
public:
    virtual ~IRenderingMethod() = default;
    virtual void beginFrame() = 0;
    virtual void submitFrame() = 0;
    virtual IFrame& frame() = 0;

    virtual bool usesOptimizedRendering() const { return false; }
};