#pragma once
#include <SystemId.hpp>
#include <string>
#include <utility>

class SystemIdPool
{
    using system_pool_id_t = SystemId::system_id_t;

public:
    static SystemId getNext(std::string name = "")
    {
        return SystemId(nextOfIdType(), std::move(name));
    }

private:
    static system_pool_id_t nextOfIdType()
    {
        static system_pool_id_t id{0u};
        return ++id;
    }
};