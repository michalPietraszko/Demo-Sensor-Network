#pragma once

#include <Event.hpp>
#include <Environment.hpp>

class EventFactory
{
public:
    template<typename T, typename...Args>
    static event_handle_t create(Args&&... args)
    {   
        auto& sharedMem = Environment::sharedMemory();
        auto temp = sharedMem.allocate<T>(std::forward<Args>(args)...);
        return sharedMem.handle_from_ptr(temp);
    }

    template<typename T>
    static T* get(event_handle_t e)
    {
        return Environment::sharedMemory().ptr_from_handle<T>(e);
    }
};