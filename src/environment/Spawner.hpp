#pragma once

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <utility>
#include <Logging.hpp>
#include <Runnable.hpp>
#include <type_traits>

namespace spawner
{
using RunnableId = pid_t;
constexpr static RunnableId runningProcess{};

template <typename T, typename... Args>
static RunnableId spawn(Args&&... args)
{
    const auto childProc = fork();
    if (childProc == 0)
    {
        LOG_INF("spawned: ", getpid(), " ", typeid(T).name());
        {
            T t(std::forward<Args>(args)...);
            t.run();
        }
        LOG_INF("process: ", getpid(), " finished running");
        return runningProcess;
    }
    return childProc;
}
} // namespace spawner