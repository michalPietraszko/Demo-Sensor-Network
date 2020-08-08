#pragma once

#include <Message.hpp>
#include <ComponentModify.hpp>
#include <SensorReportReq.hpp>

#include <Logging.hpp>
#include <MessageDispatcher.hpp>
#include <SmartSharedMessage.hpp>

class ApplicationMessageDispatcher : public MessageDispatcher
{
public:
    virtual ~ApplicationMessageDispatcher() = default;
    virtual bool dispatch(SmartSharedMessage& smartMsg) override
    {
        switch (smartMsg.get()->messageType)
        {
            case ComponentModify::staticType:
                onComponentModifyReceived(smartMsg);
                return true;
            case SensorReportReq::staticType:
                onSensorReportReq(smartMsg);
                return true;
            default:
                return false;
        }
    }

protected:
    virtual void onComponentModifyReceived(SmartSharedMessage& msg)
    {
        LOG_ERR("Unhandled ComponentModify!");
    }

    virtual void onSensorReportReq(SmartSharedMessage& msg)
    {
        LOG_ERR("Unhandled SensorReportReq!");
    }
};