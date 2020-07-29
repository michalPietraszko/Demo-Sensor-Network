#pragma once

#include <unistd.h>

class MultiProcessGuard
{
public:
    MultiProcessGuard() : m_ThisProcessId{getpid()} {}
    virtual ~MultiProcessGuard() {}

    pid_t getCreatorPid() const { return m_ThisProcessId; }
    bool isCreatorProcess(const pid_t current = getpid()) const { return current == getCreatorPid(); }

private:
    const pid_t m_ThisProcessId;
};