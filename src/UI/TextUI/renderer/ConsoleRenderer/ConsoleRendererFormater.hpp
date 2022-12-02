#pragma once
#include <FormatGlyphs.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class ConsoleRendererFormater {
public:
    static void clear() { enumerator() = 0; }

    static std::string convert(FormatGlyphs glyph) {
        switch (glyph) {
            case FormatGlyphs::newLine:
                return onNewLine();
            case FormatGlyphs::tab:
                return onTab();
            case FormatGlyphs::nextInList:
                return nextInList();
            case FormatGlyphs::element:
                throw std::runtime_error("Passed \"element\" to converter!");
            default:
                throw std::runtime_error("Unknown glyph!");
                return "";
        }
    }

private:
    static std::string onNewLine() { return "\n"; }
    static std::string onTab() { return "\t"; }
    static std::string nextInList() { return std::to_string(++enumerator()) + ". "; }

    static int& enumerator() {
        static int i = 0;
        return i;
    }
};