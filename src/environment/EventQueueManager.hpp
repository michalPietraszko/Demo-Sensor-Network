#pragma once
#include <EventMessageQueue.hpp>
#include <LifetimeLogger.hpp>
#include <Logging.hpp>
#include <MultiProcessGuard.hpp>
#include <SystemId.hpp>
#include <set>

class Environment;
class EventQueueManager : LifetimeLogger {
    friend class Environment;
    EventQueueManager() : LifetimeLogger("EventQueueManager") {}

public:
    ~EventQueueManager() {
        if (guard.isCreatorProcess()) removeAll();
    }

    std::unique_ptr<EventMessageQueue> get(const SystemId& id) { return get(id.toString().c_str()); }

    std::unique_ptr<EventMessageQueue> get(const char* name) { // clang-format off
        try { return std::make_unique<EventMessageQueue>(name); }
        catch (ipc::interprocess_exception& e) // clang-format on
        {
            const auto isInLocalStorage = m_Queues.find(name) != m_Queues.end();
            LOG_ERR("Couldn't find queue: ", name, ", is in local storage: ", std::boolalpha, isInLocalStorage);
            return {nullptr};
        }
    }

    std::unique_ptr<EventMessageQueue> add(const SystemId& id, EventMessageQueue::size_type size = 200) {
        return add(id.toString().c_str(), size);
    }

    std::unique_ptr<EventMessageQueue> add(const char* name, EventMessageQueue::size_type size = 200) {
        /* only main process can add queues */
        if (not guard.isCreatorProcess()) return {nullptr};
        if (auto wasInsertionSuccessful = m_Queues.insert(name).second; wasInsertionSuccessful) {
            remove(name); // sanity check
            LOG_INF("Created queue: ", name, ", size: ", size);
            return std::make_unique<EventMessageQueue>(name, size);
        }

        LOG_ERR("Queue: ", name, ", already exists!");
        return {nullptr};
    }

private:
    void remove(const char* name) { ipc::message_queue::remove(name); }

    void removeAll() {
        LOG_INF("Remove all with pid: ", getpid());

        for (auto it = m_Queues.cbegin(); it != m_Queues.cend();) {
            const auto name = it->c_str();
            remove(name);
            LOG_INF("Removing queue: ", name);
            m_Queues.erase(it++);
        }
    }

    std::set<std::string> m_Queues{};
    MultiProcessGuard guard{};
};
