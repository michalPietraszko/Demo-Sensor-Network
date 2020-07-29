#pragma once
#include <Environment.hpp>
#include <SystemId.hpp>
#include <SystemIdPool.hpp>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

class SystemComponents
{
    using id_type_t = SystemId;
    using queue_t = std::unique_ptr<EventMessageQueue>;

    struct Sensor
    {
        Sensor(id_type_t id, queue_t&& q, unsigned period, unsigned val)
            : id{id}, queue{std::move(q)}, reportingPeriod{period}, startingValue{val}
        {
        }

        const id_type_t id;
        queue_t queue;
        const unsigned reportingPeriod;
        const unsigned startingValue;
    };

    using Sensors = std::vector<Sensor>;

    struct MainNode
    {
        constexpr static auto name = "main-node";
        MainNode(id_type_t id, queue_t&& q, unsigned bsize) : id{id}, queue{std::move(q)}, reportingBufferSize{bsize} {}

        const id_type_t id;
        queue_t queue;
        const unsigned reportingBufferSize;
    };

public:
    template <typename... Args>
    Sensor& addSensor(const std::string& name, Args&&... args)
    {
        const auto id = SystemIdPool::getNext(name);
        auto queue = Environment::queueManager().add(id);
        sensors.emplace_back(id, std::move(queue), args...);

        return sensors.back();
    }

    template <typename... Args>
    MainNode& addMainNode(Args&&... args)
    {
        assert(not mainNode);
        const auto id = SystemIdPool::getNext(MainNode::name);
        auto queue = Environment::queueManager().add(id);
        mainNode = std::make_unique<MainNode>(id, std::move(queue), args...);
        return *mainNode;
    }

    MainNode& getMainNode()
    {
        assert(mainNode);
        return *mainNode;
    }

    Sensors& getSensors() { return sensors; }

    unsigned getNumOfSensors() const { return sensors.size(); }

    bool isMainNodeCreated() const { return mainNode != nullptr; }

    bool isAllSystemSetup() const 
    {
        return isMainNodeCreated() and getNumOfSensors();
    }

    Sensor& findSensorById(const SystemId& id)
    {
        const auto res = findSensor([&id](auto& sensor) { return sensor.id.toString() == id.toString(); });
        assert(res != sensors.end());
        return *res;
    }

private:
    template <typename FindFn>
    Sensors::iterator findSensor(FindFn fn)
    {
        return std::find_if(sensors.begin(), sensors.end(), fn);
    }

    Sensors sensors;
    std::unique_ptr<MainNode> mainNode;
};