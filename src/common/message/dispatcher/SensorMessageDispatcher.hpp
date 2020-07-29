#pragma once

#include <Message.hpp>
#include <SensorStatusReport.hpp>
#include <Logging.hpp>
#include <MessageDispatcher.hpp>

class SensorMessageDispatcher : public MessageDispatcher
{
public:
    virtual ~SensorMessageDispatcher() = default;
    virtual bool dispatch(Message* msg) override
    {
        switch (msg->messageType)
        {
            case SensorStatusReport::staticType:
                onSensorStatusReportReceived(*(static_cast<SensorStatusReport*>(msg)));
                return true;
            default:
                return false;
        }
    }

protected:
    virtual void onSensorStatusReportReceived(SensorStatusReport& statusReport)
    {
        LOG_ERR("Unhandled sensor status report!");
    }
};