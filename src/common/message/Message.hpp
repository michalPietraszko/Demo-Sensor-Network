#pragma once
#include <MessageTypes.hpp>

struct Message
{
    Message(const MessageType& type) : messageType{type} {}
    
    Message(const Message&) = default;
    Message(Message&&) = default;
    Message& operator=(const Message&) = delete;
    Message& operator=(Message&&) = delete;
    virtual ~Message() = default;
    
    const MessageType messageType{};
};