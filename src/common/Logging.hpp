#pragma once

#include <LogService.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <chrono>
#include <ctime>
#include <sstream>
#include <string>

/* MACROS */
#define LOG_INF(...) Logging::log("INF", __FILE__, __LINE__, __VA_ARGS__);
#define LOG_ERR(...) Logging::log("ERR", __FILE__, __LINE__, __VA_ARGS__);
#define LOG_INF_ID(id, ...) Logging::logId("INF", __FILE__, __LINE__, id, __VA_ARGS__);
#define LOG_ERR_ID(id, ...) Logging::logId("ERR", __FILE__, __LINE__, id, __VA_ARGS__);

/* IMPL */
class Logging;

class LogFormater {
    friend class Logging;
    LogFormater() = default;

public:
    template <typename... T>
    std::string format(const std::string time, const char* file, const int line, std::string lvl, T... params) {
        std::stringstream ss;
        ss << time << " | ";
        ss << trimFilename(file) << ": " << line << " | ";
        ss << lvl << " | ";
        ((ss << params), ...);
        return ss.str();
    }

    template <typename... T>
    std::string formatId(const std::string time,
                         const char* file,
                         const int line,
                         std::string lvl,
                         std::string id,
                         T... params) {
        std::stringstream ss;
        ss << time << " | ";
        ss << trimFilename(file) << ": " << line << " | ";
        ss << lvl << " | ";
        ss << "[" << id << "]: ";
        ((ss << params), ...);
        return ss.str();
    }

private:
    static std::string trimFilename(std::string filename) { return filename.substr(filename.rfind("/") + 1); }
};

class Logging {
    Logging() = default;

public:
    template <typename... T>
    static void log(std::string lvl, const char* file, const int line, T&&... params) {
        LogFormater formater;
        const auto formatedLog = formater.format(getTime(), file, line, std::move(lvl), std::forward<T>(params)...);
        LogService::log(formatedLog.c_str());
    }

    template <typename... T>
    static void logId(std::string lvl, const char* file, const int line, std::string id, T&&... params) {
        LogFormater formater;
        const auto formatedLog =
            formater.formatId(getTime(), file, line, std::move(lvl), id, std::forward<T>(params)...);
        LogService::log(formatedLog.c_str());
    }

private:
    static std::string getTime() {
        using namespace boost::posix_time;

        const auto now = microsec_clock::local_time();
        return to_simple_string(now);
    }
};
