#pragma once
#include <Message.hpp>
#include <Logging.hpp>

class MessageDispatcher
{
public:
    virtual ~MessageDispatcher() = default;
    virtual bool dispatch(Message* msg) = 0;

    static void onInvalidMessageReceived(Message* mgs) { LOG_ERR("Invalid message received!"); }
};