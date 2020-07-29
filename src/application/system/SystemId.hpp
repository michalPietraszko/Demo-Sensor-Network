#pragma once
#include <cassert>
#include <string>

// clang-format off
class SystemIdPool;
class SystemId
{
    friend class SystemIdPool;
    using system_id_t = unsigned;

    SystemId(system_id_t value, std::string&& name) 
        : m_Value{value},  m_Name{std::to_string(m_Value) + "-" + name}, m_PrettyName{std::move(name)} {}

public:
            int value()    const { return m_Value; }
    std::string toString() const { return m_Name; }
    std::string name()     const { return m_PrettyName; }

private:
    const system_id_t m_Value {0u};
    const std::string m_Name {""};
    const std::string m_PrettyName{""}; 
};
// clang-format on
