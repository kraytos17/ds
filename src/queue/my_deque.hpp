#pragma once

#include <optional>
#include <stdexcept>
#include <vector>

template<typename T>
class Deque {
public:
    constexpr Deque() = default;

    constexpr void pushFront(const T& value) { m_storage.insert(m_storage.begin(), value); }

    constexpr void pushBack(const T& value) { m_storage.push_back(value); }

    constexpr void popFront() {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        m_storage.erase(m_storage.begin());
    }

    constexpr void popBack() {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        m_storage.pop_back();
    }

    [[nodiscard]] constexpr bool empty() const { return m_storage.empty(); }
    [[nodiscard]] constexpr std::size_t size() const { return m_storage.size(); }

    constexpr void clear() noexcept { m_storage.clear(); }

    constexpr const T& front() const {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        return m_storage.front();
    }

    constexpr const T& back() const {
        if (empty()) {
            throw std::out_of_range("Deque is empty");
        }
        return m_storage.back();
    }

    constexpr std::optional<T> at(std::size_t idx) const {
        if (idx < m_storage.size()) {
            return m_storage[idx];
        }
        return std::nullopt;
    }

private:
    std::vector<T> m_storage;
};
