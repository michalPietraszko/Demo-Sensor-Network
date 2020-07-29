#pragma once
#include <chrono>
#include <iostream>
#include <thread>
#include <Config.hpp>

class LoopTimingController
{
    using high_resolution_clock = std::chrono::high_resolution_clock;
    using time_point_t = std::chrono::time_point<high_resolution_clock>;
    using milliseconds = std::chrono::milliseconds;

public:
    ~LoopTimingController()
    {
        waitUntilRefreshPeriod();
    }

private:
    void waitUntilRefreshPeriod()
    {
        const auto end = high_resolution_clock::now();
        std::chrono::duration<double, std::micro> timeElapsed = end - m_Start;

        if (m_LoopRefreshPeriod > timeElapsed)
        {
            const auto waitTime = m_LoopRefreshPeriod - timeElapsed;
            std::this_thread::sleep_for(waitTime);
        }
    }

    time_point_t m_Start{high_resolution_clock::now()};
    constexpr static milliseconds m_LoopRefreshPeriod{Config::appLoopPeriodMs};
};