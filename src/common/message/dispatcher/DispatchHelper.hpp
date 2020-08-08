#pragma once
#include <SmartSharedMessage.hpp>
#include <MessageDispatcher.hpp>

template<typename Self>
class DispatchHelper
{
public:
    template<typename... Dispatchers>
    static dispatch(Self& self, SmartSharedMessage& msg)
    {
        const isDispatched = (... or (static_cast<Dispatchers*>(&this)->dispatch(msg)));
        if(not isDispatched) MessageDispatcher::onInvalidMessageReceived(msg);
    }
};