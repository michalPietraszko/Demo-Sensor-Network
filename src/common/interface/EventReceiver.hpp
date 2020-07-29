#pragma once
#include <Event.hpp>
#include <Environment.hpp>
#include <SystemId.hpp>
#include <optional>
#include <EventMessageQueue.hpp>
#include <EventFactory.hpp> // temporary
#include <Message.hpp> // temporary

class EventReceiver
{
public:
    EventReceiver(const SystemId& id) : m_MessageQueue{Environment::queueManager().get(id)} {}
    ~EventReceiver()
    {
        clearQueue();
    }

    event_handle_t receive()
    {
        return m_MessageQueue->receive();
    }

    std::optional<event_handle_t> try_receive()
    {
        return m_MessageQueue->try_receive();
    }

protected:
    std::unique_ptr<EventMessageQueue> m_MessageQueue;

private:
    void clearQueue()
    {   
        auto event = try_receive();
        while(event)
        {
            // todo add fn to shared memory
            auto msg = EventFactory::get<Message>(*event);
            Environment::sharedMemory().free(msg);
            event = try_receive();
        }
    }
};