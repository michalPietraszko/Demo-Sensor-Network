#pragma once

#include <Logging.hpp>
#include <Runnable.hpp>
#include <algorithm>
#include <signal.h>
#include <string>
#include <type_traits>
#include <unistd.h>
#include <utility>

class ComponentSpawner {
public:
    template <typename T, typename... Args>
    static void spawnRunnable(Args&&... args) {
        static_assert(std::is_base_of_v<Runnable, T>, "Type is not of \"Runnable\" type!");
        const auto childProc = fork();
        if (childProc == 0) {
            auto strName = std::string(typeid(T).name());
            stripDigits(strName);
            LOG_INF("Spawned: ", strName, ", pid: ", getpid());
            {
                T t(std::forward<Args>(args)...);
                t.run();
            }
            LOG_INF("Process: ", getpid(), " finished running");
            raise(SIGKILL);
        }
    }

private:
    static void stripDigits(std::string& str) {
        str.erase(std::remove_if(str.begin(), str.end(), [](auto& ch) { return ::isdigit(ch); }));
    }
};
