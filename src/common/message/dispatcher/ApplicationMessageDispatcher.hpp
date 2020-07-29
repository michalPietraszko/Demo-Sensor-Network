#pragma once

#include <Message.hpp>
#include <ComponentModify.hpp>
#include <SensorReportReq.hpp>

#include <Logging.hpp>
#include <MessageDispatcher.hpp>

class ApplicationMessageDispatcher : public MessageDispatcher
{
public:
    virtual ~ApplicationMessageDispatcher() = default;
    virtual bool dispatch(Message* msg) override
    {
        switch (msg->messageType)
        {
            case ComponentModify::staticType:
                onComponentModifyReceived(*(static_cast<ComponentModify*>(msg)));
                return true;
            case SensorReportReq::staticType:
                onSensorReportReq(*(static_cast<SensorReportReq*>(msg)));
                return true;
            default:
                return false;
        }
    }

protected:
    virtual void onComponentModifyReceived(ComponentModify& statusReport)
    {
        LOG_ERR("Unhandled ComponentModify!");
    }

    virtual void onSensorReportReq(SensorReportReq& statusReport)
    {
        LOG_ERR("Unhandled SensorReportReq!");
    }
};