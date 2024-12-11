#pragma once

#include <array>
#include <cstddef>
#include <stdexcept>

template<typename T, std::size_t N>
class MyCircularQueue {
public:
    constexpr MyCircularQueue() = default;

    constexpr void enqueue(const T& value) {
        if (full()) {
            throw std::out_of_range("Queue is full");
        }
        m_storage[m_rear] = value;
        m_rear = (m_rear + 1) % N;
        ++m_size;
    }

    constexpr void dequeue() {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }
        m_front = (m_front + 1) % N;
        --m_size;
    }

    constexpr const T& front() const {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }
        return m_storage[m_front];
    }

    constexpr const T& back() const {
        if (empty()) {
            throw std::underflow_error("Queue is empty");
        }
        std::size_t last = (m_rear == 0) ? N - 1 : m_rear - 1;
        return m_storage[last];
    }

    constexpr bool full() const { return m_size == N; }
    constexpr bool empty() const { return m_size == 0; }
    constexpr std::size_t size() const { return m_size; }

private:
    std::array<T, N> m_storage{};
    std::size_t m_front{};
    std::size_t m_rear{};
    std::size_t m_size{};
};
