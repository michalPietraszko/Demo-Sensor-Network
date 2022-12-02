#pragma once
#include <Message.hpp>
#include <MessageTypes.hpp>

struct SensorStatusReport : Message {
    constexpr static auto staticType = MessageType::SensorStatusReport;

    SensorStatusReport(int i) : Message{staticType}, report{i} {}
    int report{};
};