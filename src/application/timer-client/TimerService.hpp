#pragma once
#include <Timer.hpp>
#include <functional>
#include <vector>

class TimerService
{
public:
    using on_timeout_fn_t = std::function<void()>;
    struct TimerAction
    {
        Timer timer;
        on_timeout_fn_t callback;
    };

    Timer& add(const Timer::timeout_ms_t timeout, const Timer::TimerType type, on_timeout_fn_t&& callback)
    {
        timerActions.push_back({{timeout, type}, std::move(callback)});
        return timerActions.back().timer;
    }

    void update()
    {
        if(not isRunning) return;

        auto current = std::begin(timerActions);
        while (current != std::end(timerActions)) 
        {
            const auto isExpired = current->timer.isExpired();   
            if (isExpired)
            {
                current->callback();
                if(not current->timer.isPeriodic())
                {
                    current = timerActions.erase(current);
                    continue;
                }
            }
            
            ++current;
        }
    }

    void start()
    {
        isRunning = true;
        for(auto& timerAction : timerActions)
        {
            timerAction.timer.start();
        }
    }

    void stop()
    {
        isRunning = false;
        for(auto& timerAction : timerActions)
        {
            timerAction.timer.reset();
        }
    }

private:
    using TimerActions = std::vector<TimerAction>;
    TimerActions timerActions;
    bool isRunning{false};
};