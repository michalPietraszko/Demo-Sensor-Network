#pragma once
#include <Environment.hpp>
#include <Event.hpp>
#include <SystemId.hpp>

class EventSender {
public:
    EventSender(const SystemId& id) : m_MessageQueue{Environment::queueManager().get(id)} {}

    void send(event_handle_t& event) { m_MessageQueue->send(event); }

protected:
    std::unique_ptr<EventMessageQueue> m_MessageQueue;
};