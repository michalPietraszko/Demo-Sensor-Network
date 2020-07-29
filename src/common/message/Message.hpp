#pragma once
#include <MessageTypes.hpp>

struct Message
{
    Message(const MessageType& type) : messageType{type} {}
    virtual ~Message() = default;
    
    const MessageType messageType{};
};