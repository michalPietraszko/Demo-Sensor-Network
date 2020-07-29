#pragma once
#include <EventQueueManager.hpp>
#include <SharedMemory.hpp>
#include <SpawnedManager.hpp>
#include <LogService.hpp>

class Environment
{
public:
    static EventQueueManager& queueManager()
    {
        return queueMgr;
    }

    static SharedMemory& sharedMemory()
    {
        return memory;
    }

    static SpawnedManager& spawnedManager()
    {
        return spawnedMgr;
    }

private:
    static LogService loggingService;
    static SharedMemory memory;
    static SpawnedManager spawnedMgr;
    static EventQueueManager queueMgr;
};