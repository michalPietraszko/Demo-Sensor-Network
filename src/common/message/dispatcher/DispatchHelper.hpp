#pragma once
#include <Message.hpp>
#include <MessageDispatcher.hpp>

template<typename Self>
class DispatchHelper
{
public:
    template<typename... Dispatchers>
    static dispatch(Self& self, Message* msg)
    {
        const isDispatched = (... or (static_cast<Dispatchers*>(&this)->dispatch(msg)));
        if(not isDispatched) MessageDispatcher::onInvalidMessageReceived(msg);
    }
};