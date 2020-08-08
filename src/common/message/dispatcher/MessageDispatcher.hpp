#pragma once
#include <Logging.hpp>
#include <SmartSharedMessage.hpp>

class MessageDispatcher
{
public:
    virtual ~MessageDispatcher() = default;
    virtual bool dispatch(SmartSharedMessage& msg) = 0;

    static void onInvalidMessageReceived(SmartSharedMessage& mgs) { LOG_ERR("Invalid message received!"); }
};