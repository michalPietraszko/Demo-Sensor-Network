#pragma once 
#include <string>
#include <array>

class UITextInput
{
static constexpr auto size = 100;
public:
    UITextInput() = default;
    auto& getBuffer() { return storage; }
    bool& getInputPendingFlag() {return m_HasInputPending; }
private:
    std::array<char, size> storage{};
    bool m_HasInputPending{false};
};