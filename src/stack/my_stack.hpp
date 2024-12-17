#pragma once

#include <algorithm>
#include <concepts>
#include <forward_list>
#include <stdexcept>
#include <vector>

template<typename T>
concept Stackable = std::movable<T>;

template<Stackable T>
class Stack {
public:
    constexpr Stack() = default;

    constexpr void push(const T& value) {
        m_storage.push_front(value);
        ++m_size;
    }

    constexpr void push(T&& value) {
        m_storage.push_front(std::move(value));
        ++m_size;
    }

    constexpr void pop() {
        if (empty()) {
            throw std::out_of_range("Stack underflow: Cannot pop from an empty stack.");
        }
        m_storage.pop_front();
        --m_size;
    }

    constexpr T& top() {
        if (empty()) {
            throw std::out_of_range("Stack is empty: No top element.");
        }
        return m_storage.front();
    }

    [[nodiscard]] constexpr const T& top() const {
        if (empty()) {
            throw std::out_of_range("Stack is empty: No top element.");
        }
        return m_storage.front();
    }

    [[nodiscard]] constexpr bool empty() const noexcept { return m_size == 0; }
    [[nodiscard]] constexpr std::size_t size() const noexcept { return m_size; }

    constexpr void clear() noexcept {
        m_storage.clear();
        m_size = 0;
    }

    constexpr auto elements() const noexcept {
        std::vector<T> result;
        result.reserve(m_size);
        std::ranges::copy(m_storage, std::back_inserter(result));

        return result;
    }

private:
    std::forward_list<T> m_storage;
    std::size_t m_size{0};
};
