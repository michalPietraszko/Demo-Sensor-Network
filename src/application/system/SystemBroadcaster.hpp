#pragma once
#include <EventFactory.hpp>
#include <SystemComponents.hpp>
#include <SystemId.hpp>
#include <message/definitions/ComponentModify.hpp>
#include <message/definitions/SensorReportReq.hpp>
#include <message/definitions/SensorStatusReport.hpp>

class SystemBroadcaster {
public:
    SystemBroadcaster(SystemComponents& components) : m_Components{components} {}

    void broadcastStart() { boradcast<ComponentModify>(ComponentModify::Cause::componentEnable); }

    void boradcastStop() { boradcast<ComponentModify>(ComponentModify::Cause::componentDelete); }

    void sendSensorReportTimerEvent(const SystemId& id) {
        m_Components.findSensorById(id).queue->send(EventFactory::create<SensorReportReq>());
    }

    void sendStart(const SystemId& id) {
        send<ComponentModify>(m_Components.findSensorById(id), ComponentModify::Cause::componentEnable);
    }

private:
    template <typename Msg, typename... Args>
    void boradcast(Args&&... args) {
        m_Components.getMainNode().queue->send(EventFactory::create<Msg>(args...));
        auto& sensors = m_Components.getSensors();
        for (auto& sensor : sensors) {
            sensor.queue->send(EventFactory::create<Msg>(args...));
        }
    }

    template <typename Msg, typename Type, typename... Args>
    void send(Type& component, Args&&... args) {
        component.queue->send(EventFactory::create<Msg>(args...));
    }

    SystemComponents& m_Components;
};