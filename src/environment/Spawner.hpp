#pragma once

#include <Logging.hpp>
#include <Runnable.hpp>
#include <iostream>
#include <signal.h>
#include <type_traits>
#include <unistd.h>
#include <utility>

namespace spawner {
using RunnableId = pid_t;
constexpr static RunnableId runningProcess{};

template <typename T, typename... Args>
static RunnableId spawn(Args&&... args) {
    const auto childProc = fork();
    if (childProc == 0) {
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