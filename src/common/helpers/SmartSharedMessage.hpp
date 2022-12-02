#pragma once
#include <Environment.hpp>
#include <Event.hpp>
#include <EventFactory.hpp>
#include <Message.hpp>
#include <memory>

class SmartSharedMessage {
public:
    SmartSharedMessage(Message* msg) : m_OwnedMessage{msg, deleter} {}
    SmartSharedMessage(event_handle_t& event) : m_OwnedMessage{EventFactory::get<Message>(event), deleter} {}

    [[nodiscard]] Message* release() { return m_OwnedMessage.release(); }

    template <typename T = Message>
    [[nodiscard]] T* get() {
        return static_cast<T*>(m_OwnedMessage.get());
    }

private:
    inline static auto deleter = [](auto* msg) { Environment::sharedMemory().free(msg); };

    std::unique_ptr<Message, decltype(deleter)> m_OwnedMessage;
};