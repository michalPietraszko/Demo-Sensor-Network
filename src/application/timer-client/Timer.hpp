#pragma once
#include <chrono>

class Timer {
    using high_resolution_clock = std::chrono::high_resolution_clock;
    using time_point_t = std::chrono::time_point<high_resolution_clock>;
    using milliseconds = std::chrono::milliseconds;

public:
    using timeout_ms_t = milliseconds;
    enum class TimerType { oneShot, periodic };

    Timer(const milliseconds timeout, TimerType type = TimerType::oneShot)
        : m_Timeout{timeout}, m_IsPeriodic{type == TimerType::periodic} {}

    void start() {
        if (m_IsRunning) return;

        m_Start = high_resolution_clock::now();
        m_IsRunning = true;
        m_WasStarted = true;
    }

    void reset() {
        m_IsRunning = false;
        m_WasStarted = false;
    }

    bool isExpired() {
        if (not m_WasStarted) return false;
        if (not m_IsRunning) return true;

        const auto now = high_resolution_clock::now();
        const auto timeElapsed = std::chrono::duration_cast<milliseconds>(now - m_Start);

        if (timeElapsed > m_AdjustedTimeout) /* is expired */
        {
            reset();
            if (m_IsPeriodic) restart(timeElapsed);

            return true;
        }

        return false;
    }

    bool isPeriodic() const { return m_IsPeriodic; }

private:
    void restart(const milliseconds timeSinceTimerStart) {
        adjustTimeout(timeSinceTimerStart);
        start();
    }

    void adjustTimeout(const milliseconds timeSinceTimerStart) {
        const auto timeOverTimeout = timeSinceTimerStart - m_AdjustedTimeout;
        m_AdjustedTimeout = m_Timeout - timeOverTimeout;
    }

private:
    time_point_t m_Start;

    milliseconds m_Timeout;
    milliseconds m_AdjustedTimeout{m_Timeout};

    bool m_IsPeriodic{false};
    bool m_IsRunning{false};
    bool m_WasStarted{false};
};