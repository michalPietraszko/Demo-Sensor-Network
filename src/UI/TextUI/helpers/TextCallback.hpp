#include <functional>
#include <optional>
#include <string>
#include <utility>

class TextCallback {
    using TextCallbackFn = std::function<std::string()>;

public:
    TextCallback() = default;
    TextCallback(TextCallbackFn&& fn) : textCallback(std::move(fn)) {}

    void setTextCallback(TextCallbackFn&& fn) { textCallback = std::move(fn); }

    void resetTextCallback() {
        textCallback = []() { return ""; };
    }

    std::string getTextCallback() const { return textCallback(); }

private:
    TextCallbackFn textCallback = []() { return ""; };
};