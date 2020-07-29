#pragma once
#include <unistd.h>
#include <Logging.hpp>
#include <string>

class LifetimeLogger
{
public:
    LifetimeLogger(std::string name="") : m_Name{std::move(name)}
    {
        LOG_INF("Created: ", m_Name,", pid: ", getpid());
    }

    virtual ~LifetimeLogger()
    {
        LOG_INF("Destroying: ", m_Name, ", pid: ", getpid());
    }

private:
    const std::string m_Name = "";
};
