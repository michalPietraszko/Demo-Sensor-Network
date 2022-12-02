#pragma once
#include <Message.hpp>
#include <MessageTypes.hpp>

struct ComponentModify: Message {
    enum class Cause { 
        componentEnable, 
        componentDelete 
    };

    constexpr static auto staticType =
        MessageType::ComponentModify;

    ComponentModify(Cause c) 
        : Message{staticType}, cause{c} 
    {}

    const Cause cause;
};