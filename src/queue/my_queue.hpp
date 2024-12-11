#pragma once

#include <deque>
#include <optional>
#include <stdexcept>

template<typename T>
class MyQueue {
public:
    constexpr MyQueue() = default;

    constexpr void enqueue(const T& value) { m_storage.push_back(value); }

    constexpr void dequeue() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        m_storage.pop_front();
    }

    constexpr const T& front() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return m_storage.front();
    }

    constexpr const T& back() const {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return m_storage.back();
    }

    constexpr std::optional<T> at(const std::size_t idx) const {
        if (idx < m_storage.size()) {
            return m_storage[idx];
        }
        return std::nullopt;
    }

    constexpr void clear() noexcept { m_storage.clear(); }

    [[nodiscard]] constexpr bool empty() const { return m_storage.empty(); }
    [[nodiscard]] constexpr std::size_t size() const { return m_storage.size(); }

    [[nodiscard]] constexpr auto begin() { return m_storage.begin(); }
    [[nodiscard]] constexpr auto end() { return m_storage.end(); }
    [[nodiscard]] constexpr auto cbegin() const { return m_storage.cbegin(); }
    [[nodiscard]] constexpr auto cend() const { return m_storage.cend(); }
    [[nodiscard]] constexpr auto rbegin() { return m_storage.rbegin(); }
    [[nodiscard]] constexpr auto rend() { return m_storage.rend(); }
    [[nodiscard]] constexpr auto crbegin() const { return m_storage.crbegin(); }
    [[nodiscard]] constexpr auto crend() const { return m_storage.crend(); }

private:
    std::deque<T> m_storage;
};
