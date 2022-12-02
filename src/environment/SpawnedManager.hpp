#pragma once
#include <LifetimeLogger.hpp>
#include <Logging.hpp>
#include <MultiProcessGuard.hpp>
#include <Spawner.hpp>
#include <signal.h>
#include <unistd.h>
#include <vector>

class SpawnedManager : public MultiProcessGuard, public LifetimeLogger {
    using Children = std::vector<pid_t>;

public:
    SpawnedManager() : LifetimeLogger("SpawnedMgr") { signal(SIGQUIT, SpawnedManager::sigQuitHandler); }

    void finalize() {
        if (not isCreatorProcess()) return;

        kill(-parentPid, SIGQUIT);
    }

    void killOnFinish(spawner::RunnableId id) {
        if (id == spawner::runningProcess) {
            raise(SIGKILL);
        }
    }

    void pauseOnFinish(spawner::RunnableId id) {
        if (id == spawner::runningProcess) {
            raise(SIGSTOP);
        }
        add(id);
    }

private:
    void add(pid_t id) {
        if (not isCreatorProcess()) return;
        children.push_back(id);
        LOG_INF("Added: ", id);
    }

    void remove(pid_t id) {
        if (not isCreatorProcess()) return;
        children.erase(std::remove(children.begin(), children.end(), id), children.end());
    }

    /*void resume_test()
    {
        if (not isCreatorProcess()) return;

        for(auto& child : children)
        {
            kill(child, SIGCONT);
        }
    }*/

    static void sigQuitHandler(int signal) {
        assert(signal == SIGQUIT);
        if (parentPid != getpid()) _exit(0);
    }

public:
    const static pid_t parentPid;

private:
    Children children;
};
