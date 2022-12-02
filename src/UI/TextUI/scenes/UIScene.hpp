#pragma once
#include <InputResult.hpp>
#include <Renderer.hpp>
#include <UIAdapter.hpp>
#include <utility>

// class UIAdapter;

class UIScene {
public:
    virtual ~UIScene() = default;

    virtual InputResult processUserInput() = 0;
    virtual void update(UIAdapter& adapter) = 0;
    virtual void load(UIAdapter& adapter) = 0;
    virtual void display(Renderer& renderer) = 0;
};