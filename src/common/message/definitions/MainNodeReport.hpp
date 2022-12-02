#pragma once

#include <Event.hpp>
#include <Message.hpp>
#include <MessageTypes.hpp>
#include <algorithm>
#include <array>
#include <initializer_list>

template <auto maxSize = 10u>
struct MainNodeReport : Message {
    constexpr static auto staticType = MessageType::MainNodeReport;

    MainNodeReport(std::initializer_list<event_handle_t> messages)
        : Message{staticType}, actualSize{messages.size()}, report(event_null_handle) {
        std::move(messages.begin(), messages.end(), report.begin());
    }

    std::size_t actualSize;
    std::array<event_handle_t, maxSize> report;
};
