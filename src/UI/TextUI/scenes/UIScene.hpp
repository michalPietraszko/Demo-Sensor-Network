#pragma once
#include <UIAdapter.hpp>
#include <Renderer.hpp>
#include <utility>
#include <InputResult.hpp>

// class UIAdapter;

class UIScene
{
public:
    virtual ~UIScene() = default;

    virtual InputResult processUserInput() = 0;
    virtual void update(UIAdapter& adapter) = 0;
    virtual void load(UIAdapter& adapter) = 0;
    virtual void display(Renderer& renderer) = 0;
};