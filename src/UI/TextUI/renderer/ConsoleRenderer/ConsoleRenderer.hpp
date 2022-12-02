#pragma once
#include <ConsoleInputReader.hpp>
#include <ConsoleRendererFormater.hpp>
#include <FormatGlyphs.hpp>
#include <IFrame.hpp>
#include <IRenderingMethod.hpp>
#include <UIElements/UITextField.hpp>
#include <UIElements/UITextInput.hpp>
#include <cstring>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class ConsoleRenderer : public IRenderingMethod {
    class Frame: public IFrame {
    public:
        void setFormat(const std::vector<FormatGlyphs>& newFormat) override { currentFormat = &newFormat; }
        void add(const UITextField& elem) override { addOutput(elem.getText()); }
        void add(UITextInput& elem) override { addInput(elem); }

        void clearInputBuffer() { input.reset(nullptr); }

        void clearOutputBuffer() {
            ss.str(std::string(""));
            ConsoleRendererFormater::clear();
        }

        std::string getOutput() { return ss.str(); }
        void promptInput() {
            if (input) input->update();
        }

    private:
        void addOutput(std::string str) {
            for (auto& glyph : *currentFormat) {
                if (glyph == FormatGlyphs::element) {
                    ss << str;
                    continue;
                }
                ss << ConsoleRendererFormater::convert(glyph);
            }
        }

        /**
         *   TODO: change input to be cleared upon clear,
         *   every time 'update' is called injected TextInput will be
         *   updated
         **/
        void addInput(UITextInput& elem) {
            input.reset(nullptr);
            input = std::make_unique<ConsoleInputReader>(elem);
        }

        std::stringstream ss;
        const std::vector<FormatGlyphs>* currentFormat;
        std::unique_ptr<ConsoleInputReader> input;
    };

public:
    void beginFrame() override {
        clearScreen();
        m_Frame.clearInputBuffer();
    }

    void submitFrame() override {
        displayOutput();
        displayInput();
        m_Frame.clearOutputBuffer();
    }

    Frame& frame() override { return m_Frame; }

    bool usesOptimizedRendering() const override { return true; }

private:
    void clearScreen() const { system("clear"); }

    void displayOutput() { std::cout << m_Frame.getOutput() << std::flush; }

    void displayInput() { m_Frame.promptInput(); }

    Frame m_Frame;
};