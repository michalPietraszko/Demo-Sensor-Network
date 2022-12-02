#pragma once

#include <Message.hpp>
#include <MessageTypes.hpp>

struct SensorReportReq : Message {
    constexpr static auto staticType = MessageType::SensorReportReq;

    SensorReportReq() : Message{staticType} {}
};