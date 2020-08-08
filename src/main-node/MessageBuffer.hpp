#pragma once
#include <boost/circular_buffer.hpp>

template <typename MessageType>
class MessageBuffer
{
    using Buffer = boost::circular_buffer<MessageType>;

public:
    MessageBuffer(typename Buffer::size_type size) : m_Buffer(size) {}

    void push(MessageType&& message) { m_Buffer.push_back(std::move(message)); }

    MessageType pop_front() { return m_Buffer.pop_front(); }

    typename Buffer::size_type size() const { return m_Buffer.size(); }

private:
    Buffer m_Buffer;
};