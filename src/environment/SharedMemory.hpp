#pragma once
#include <LifetimeLogger.hpp>
#include <Logging.hpp>
#include <MultiProcessGuard.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>
#include <string>
#include <utility>

namespace ipc = boost::interprocess;

class SharedMemory : LifetimeLogger
{
public:
    using SizeInBytes = unsigned;
    using AllocatedMemoryPtr = void*;
    using Handle = ipc::managed_shared_memory::handle_t;

public:
    SharedMemory(const SizeInBytes size, std::string name = "SharedMemory1")
        : LifetimeLogger("SharedMemory, size: " + std::to_string(size))
        , sharedMemObjName{std::move(name)}
        , segment(ipc::open_or_create, sharedMemObjName.c_str(), size)
        , initFreeMemory{segment.get_free_memory()}
    {
        LOG_INF("Free mem: ", segment.get_free_memory());
    }

    ~SharedMemory()
    {
        if (guard.isCreatorProcess()) removeSegment();
    }

public:
    template <typename T, typename... Args>
    T* allocate(const char* name, Args... args)
    {
        return segment.construct<T>(name)(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    T* allocate_or_find(const char* name, Args... args)
    {
        return segment.find_or_construct<T>(name)(std::forward<Args>(args)...);
    }

    template <typename T>
    decltype(auto) find(const char* name)
    {
        return segment.find<T>(name);
    }

    template <typename T, typename... Args>
    T* allocate(Args... args)
    {
        return segment.construct<T>(ipc::anonymous_instance)(std::forward<Args>(args)...);
    }

    template <typename T>
    Handle handle_from_ptr(T* ptr)
    {
        auto vPtr = reinterpret_cast<void*>(ptr);
        return segment.get_handle_from_address(vPtr);
    }

    template <typename T>
    T* ptr_from_handle(Handle handle)
    {
        return reinterpret_cast<T*>(segment.get_address_from_handle(handle));
    }

    template <typename T>
    void free(T* ptr)
    {
        segment.destroy_ptr(ptr);
    }

private:
    void removeSegment()
    {
        logIfMemoryLeak();
        LOG_INF("Removing memory, name: ", sharedMemObjName);
        ipc::shared_memory_object::remove(sharedMemObjName.c_str());
    }

    void logIfMemoryLeak() const
    {
        if (initFreeMemory != segment.get_free_memory())
            LOG_ERR("Shared memory leak! Initial: ", initFreeMemory, ", Current:", segment.get_free_memory());
    }

    const std::string sharedMemObjName;
    ipc::managed_shared_memory segment;
    const ipc::managed_shared_memory::size_type initFreeMemory;
    MultiProcessGuard guard{};
};
