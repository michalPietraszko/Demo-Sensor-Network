#pragma once
#include <MessageDispatcher.hpp>
#include <SmartSharedMessage.hpp>

template <typename Self>
class DispatchHelper {
public:
    template <typename... Dispatchers>
    static dispatch(Self& self, SmartSharedMessage& msg) {
        const isDispatched = (... or (static_cast<Dispatchers*>(&self)->dispatch(msg)));
        if (not isDispatched) MessageDispatcher::onInvalidMessageReceived(msg);
    }
};