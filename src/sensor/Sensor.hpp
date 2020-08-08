#pragma once
#include <ApplicationMessageDispatcher.hpp>
#include <Environment.hpp>
#include <EventFactory.hpp>
#include <EventReceiver.hpp>
#include <EventSender.hpp>
#include <SmartSharedMessage.hpp>
#include <Runnable.hpp>
#include <SystemId.hpp>
#include <chrono>
#include <common/message/MessageTypes.hpp>
#include <common/message/definitions/ComponentModify.hpp>
#include <common/message/definitions/SensorStatusReport.hpp>
#include <iostream>
#include <thread>

namespace ipc = boost::interprocess;

class Sensor : public EventSender, public EventReceiver, public ApplicationMessageDispatcher, public Runnable
{
public:
    Sensor(const SystemId& self, const SystemId mainNode, unsigned startValue = 10)
        : EventSender(mainNode), EventReceiver(self), m_StartingValue{startValue}, id{self.name()}
    {
    }

    void run() override
    {
        shouldRun.store(true);
        while (shouldRun.load())
        {
            receiveNext();
        }
    }

    void sendSensorStatusReport()
    {
        static int value = m_StartingValue;
        LOG_INF_ID(id, "Sending SensorStatusReport, value = ", value);

        event_handle_t event = EventFactory::create<SensorStatusReport>(value);
        EventSender::send(event);
        value++;
    }

protected:
    virtual void onComponentModifyReceived(SmartSharedMessage& msg) override
    {
        auto modify = msg.get<ComponentModify>();
        LOG_INF_ID(id, "Received ComponentModify, cause: ", static_cast<unsigned>(modify->cause));
        switch (modify->cause)
        {
            case ComponentModify::Cause::componentEnable:
                onEnable(); /* here a function that will try-receive and collect sensor data */
                return;
            case ComponentModify::Cause::componentDelete:
                onDisable();
                return;
            default:
                LOG_ERR_ID(id, "Invalid ComponentModify cause!");
        }
    }

    virtual void onSensorReportReq(SmartSharedMessage& msg) override
    {
        auto statusReport = msg.get<SensorReportReq>();
        LOG_INF_ID(id, "Received SensorReportReq");
        if (not isEnabled)
        {
            LOG_ERR_ID(id, "SensorReportReq before ComponentModify with Enable cause");
            return;
        }
        sendSensorStatusReport();
    }

private:
    void receiveNext()
    {
        auto event = EventReceiver::receive();
        auto msg = SmartSharedMessage(event);
        handleMessage(msg);
    }

    void handleMessage(SmartSharedMessage& msg)
    {
        const auto isHandled = ApplicationMessageDispatcher::dispatch(msg);
        if(not isHandled) MessageDispatcher::onInvalidMessageReceived(msg);
    }

    void finishRunning()
    {
        shouldRun.store(false);
        LOG_INF_ID(id, "Dying");
    }

    void onEnable()  { isEnabled = true; }
    void onDisable() { isEnabled = false; finishRunning(); }

private:
    bool isEnabled{false};
    std::atomic<bool> shouldRun;

    const unsigned m_StartingValue;
    const std::string id;
};