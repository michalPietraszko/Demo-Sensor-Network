#pragma once
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <filesystem>
#include <fstream>
#include <unistd.h>
#include <utility>

namespace ipc = boost::interprocess;
namespace fs = std::filesystem;

class Logging;
class Environment;
class LogService
{
    friend class Logging;
    friend class Environment;

    constexpr static auto mutexName = "ipcMutexLog";
    constexpr static auto logsDir = "./logs";
    constexpr static auto logsFile = "./logs/syslog.log";

    LogService()
    {
        fs::create_directories(logsDir);
        
        /* initializeLogFile = */ []() {
            std::ofstream ofs;
            ofs.open(logsFile, std::ofstream::out | std::ofstream::trunc);
            ofs.close();
        }();

        ipc::named_mutex mutex(ipc::open_or_create, mutexName);
    }

    ~LogService() { ipc::named_mutex::remove(mutexName); }

private:
    static void log(const char* str)
    {
        ipc::named_mutex mutex(ipc::open_or_create, mutexName);
        ipc::scoped_lock<ipc::named_mutex> lock(mutex);

        std::ofstream myfile;
        myfile.open(logsFile, std::ofstream::app);
        myfile << str << std::endl;
        myfile.close();
    }
};