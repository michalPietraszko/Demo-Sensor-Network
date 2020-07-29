#pragma once

#include <boost/interprocess/ipc/message_queue.hpp>
#include <optional>

namespace ipc = boost::interprocess;

template <typename T>
class MessageQueue
{
    struct MessageWrapper
    {
        using size_type = ipc::message_queue::size_type;
        T content{};
        unsigned int priority{};
        size_type size{};
    };

public:
    using size_type = std::size_t;

public:
    MessageQueue(const char* queueName, size_type maxSize)
        : messageQueue{ipc::open_or_create, queueName, maxSize, sizeof(T)}
    {
    }

    MessageQueue(const char* queueName) : messageQueue{ipc::open_only, queueName} {}

    T receive()
    {
        MessageWrapper msg{};
        messageQueue.receive(&msg.content, sizeof(msg.content), msg.size, msg.priority);
        return msg.content;
    }

    std::optional<T> try_receive()
    {
        MessageWrapper msg{};
        return messageQueue.try_receive(&msg.content, sizeof(msg.content), msg.size, msg.priority) 
            ? std::make_optional<T>(msg.content)
            : std::nullopt;
    }

    void send(T msg, unsigned int priority = 0) { messageQueue.send(&msg, sizeof(msg), priority); }

private:
    ipc::message_queue messageQueue;
};