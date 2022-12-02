#pragma once
#include <TextCallback.hpp>

class UITextField : public TextCallback {
public:
    UITextField() = default;
    UITextField(std::string str) : text(std::move(str)) {}

    void setText(const std::string& str) { text = str; }
    std::string getText() const { return text + getTextCallback(); }

private:
    std::string text = "";
};