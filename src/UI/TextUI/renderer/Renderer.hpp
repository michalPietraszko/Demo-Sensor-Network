#pragma once
#include <IRenderingMethod.hpp>
#include <memory>

class Renderer
{
public:
    Renderer(std::unique_ptr<IRenderingMethod> method) : m_Renderer{std::move(method)} {}

    void newFrame() { m_Renderer->beginFrame(); }

    void flush() { m_Renderer->submitFrame(); }

    template <typename Elem>
    void submit(Elem& elem, const std::vector<FormatGlyphs>& elemFormat)
    {
        auto& frame = m_Renderer->frame();
        frame.setFormat(elemFormat);
        frame.add(elem);
    }

    bool usesOptimizedRendering() const
    {
        return m_Renderer->usesOptimizedRendering();
    }

private:
    std::unique_ptr<IRenderingMethod> m_Renderer;
};