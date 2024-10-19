#pragma once

#include <utility>

template<typename T>
class Node {
public:
    constexpr Node(T it, Node* inn) noexcept : m_data(std::move(it)), m_next(inn) {}
    constexpr explicit Node(Node* inn) noexcept : m_next(inn) {}

    [[nodiscard]] constexpr T element() const noexcept { return m_data; }
    constexpr T setElement(T it) noexcept { return m_data = std::move(it); }

    [[nodiscard]] constexpr Node* next() const noexcept { return m_next; }
    constexpr Node* setNext(Node* inn) noexcept { return m_next = inn; }

private:
    T m_data{};
    Node* m_next{nullptr};
};
