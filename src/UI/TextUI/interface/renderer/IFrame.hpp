#pragma once

#include <FormatGlyphs.hpp>
#include <UIElements/UITextField.hpp>
#include <UIElements/UITextInput.hpp>

class IFrame {
public:
    virtual ~IFrame() = default;
    virtual void setFormat(const std::vector<FormatGlyphs>& newFormat) = 0;
    virtual void add(const UITextField& elem) = 0;
    virtual void add(UITextInput& elem) = 0;
};
