#pragma once
#include <SharedMemory.hpp>

using event_handle_t = SharedMemory::Handle;
constexpr inline event_handle_t event_null_handle{};