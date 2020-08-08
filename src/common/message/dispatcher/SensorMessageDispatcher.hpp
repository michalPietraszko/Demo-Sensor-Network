#pragma once

#include <Message.hpp>
#include <SensorStatusReport.hpp>
#include <Logging.hpp>
#include <MessageDispatcher.hpp>
#include <SmartSharedMessage.hpp>

class SensorMessageDispatcher : public MessageDispatcher
{
public:
    virtual ~SensorMessageDispatcher() = default;
    virtual bool dispatch(SmartSharedMessage& msg) override
    {
        switch (msg.get()->messageType)
        {
            case SensorStatusReport::staticType:
                onSensorStatusReportReceived(msg);
                return true;
            default:
                return false;
        }
    }

protected:
    virtual void onSensorStatusReportReceived(SmartSharedMessage& msg)
    {
        LOG_ERR("Unhandled sensor status report!");
    }
};