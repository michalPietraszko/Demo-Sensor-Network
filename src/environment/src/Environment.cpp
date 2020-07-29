#include <Environment.hpp>

const pid_t SpawnedManager::parentPid = getpid();

// clang-format off
LogService        Environment::loggingService = LogService();
SpawnedManager    Environment::spawnedMgr     = SpawnedManager();
EventQueueManager Environment::queueMgr       = EventQueueManager();
SharedMemory      Environment::memory         = SharedMemory(0x2000);
// clang-format on
