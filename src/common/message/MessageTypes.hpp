#pragma once

enum class MessageType : unsigned {
    Error = 0,
    SensorStatusReport,
    SensorReportReq,
    ComponentModify,
    MainNodeReport,
};