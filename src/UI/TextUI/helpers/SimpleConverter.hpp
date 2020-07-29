#pragma once
#include <cassert>
#include <optional>
#include <unordered_map>

template <typename Input, typename Output>
class SimpleConverter
{
public:
    using Conversions = std::unordered_map<Input, Output>;

    SimpleConverter(const Output defaultOutput) : m_DefaultOutput{defaultOutput} {}

    void add(Input&& input, Output&& output)
    {
        const auto res = conversions.insert({std::move(input), std::move(output)});
        assert(res.second);
    }

    Output convert(const Input& input)
    {
        if (auto res = conversions.find(input); res != conversions.end())
        {
            return res->second;
        }
        return m_DefaultOutput;
    }

    void setDefaultOutput(const Output defaultOutput) { m_DefaultOutput = defaultOutput; }

    void clear()
    {
        conversions.clear();
    }

private:
    Conversions conversions;
    const Output m_DefaultOutput{};
};