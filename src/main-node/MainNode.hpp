#pragma once

#include <ApplicationMessageDispatcher.hpp>
#include <Environment.hpp>
#include <EventFactory.hpp>
#include <EventReceiver.hpp>
#include <Logging.hpp>
#include <MessageDispatcher.hpp>
#include <Runnable.hpp>
#include <SensorMessageDispatcher.hpp>
#include <SystemId.hpp>
#include <atomic>
#include <common/message/Message.hpp>
#include <common/message/MessageTypes.hpp>
#include <common/message/definitions/ComponentModify.hpp>
#include <common/message/definitions/SensorStatusReport.hpp>

namespace ipc = boost::interprocess;

class MainNode : public EventReceiver,
                 public SensorMessageDispatcher,
                 public ApplicationMessageDispatcher,
                 public Runnable
{
public:
    MainNode(const SystemId& id) : EventReceiver(id) {}

    void run() override
    {
        shouldRun.store(true);
        while (shouldRun.load())
        {
            receiveNext();
        }
    }

protected:
    virtual void onSensorStatusReportReceived(SensorStatusReport& statusReport) override
    {
        if (not isEnabled)
        {
            LOG_ERR_ID(id, "SensorStatusReport before ComponentModify with Enable cause!");
            return;
        }

        LOG_INF_ID(id, "Received SensorStatusReport, value: ", statusReport.report);
    }

    virtual void onComponentModifyReceived(ComponentModify& componentModify) override
    {
        LOG_INF_ID(id, "Received ComponentModify, cause: ", static_cast<unsigned>(componentModify.cause));
        switch (componentModify.cause)
        {
            case ComponentModify::Cause::componentEnable:
                onEnable();
                return;
            case ComponentModify::Cause::componentDelete:
                onDisable();
                return;
            default:
                LOG_ERR_ID(id, "Invalid ComponentModify cause!");
        }
    }

private:
    void receiveNext()
    {
        auto event = EventReceiver::receive();
        auto msg = EventFactory::get<Message>(event);
        handleMessage(msg);
        /* temporary */
        Environment::sharedMemory().free(msg);
    }

    void handleMessage(Message* msg)
    {
        const auto isHandled = 
            SensorMessageDispatcher::dispatch(msg) or ApplicationMessageDispatcher::dispatch(msg);

        if(not isHandled) MessageDispatcher::onInvalidMessageReceived(msg);
    }

    void finishRunning()
    {
        shouldRun.store(false);
        LOG_INF_ID(id, "Dying");
    }

    void onEnable()  { isEnabled = true; }
    void onDisable() { isEnabled = false; finishRunning(); }

    bool isEnabled{false};
    std::atomic<bool> shouldRun;
    const std::string id = std::string("Main Node");
};
