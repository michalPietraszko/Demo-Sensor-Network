#pragma once
#include <BasicFormater.hpp>
#include <Renderer.hpp>
#include <UITextField.hpp>
#include <UITextInput.hpp>
#include <optional>
// debug
#include <string>

class UserInputView {
public:
    UITextInput& get() { return userInputBuffer; }

    std::optional<UITextField> popInput() {
        if (not hasUnconsumedUserInput()) return {};

        UITextField userInput;
        userInput.setText(userInputBuffer.getBuffer().data());

        reset();
        return {userInput};
    }

    // debug
    std::string getInput() { return std::string(userInputBuffer.getBuffer().data()); }

    void display(Renderer& renderer) { renderer.submit(userInputBuffer, BasicFormater::any); }

private:
    bool hasUnconsumedUserInput() { return userInputBuffer.getInputPendingFlag(); }

    void reset() {
        userInputBuffer = UITextInput();
        userInputBuffer.getInputPendingFlag() = false;
    }

private:
    UITextInput userInputBuffer;
};