#pragma once

#include <functional>
#include <stdexcept>
#include <vector>
#include <iostream>

template<typename T, typename Compare = std::less<>>
class MonotonicStack {
public:
    MonotonicStack() = default;

    void push(const T& value) {
        while (!m_storage.empty() && Compare()(value, m_storage.back())) {
            m_storage.pop_back();
        }
        m_storage.push_back(value);
    }

    void pop() {
        if (!m_storage.empty()) {
            m_storage.pop_back();
        } else {
            throw std::out_of_range("Stack is empty");
        }
    }

    const T& top() const {
        if (!m_storage.empty()) {
            return m_storage.back();
        }
        throw std::out_of_range("Stack is empty");
    }

    auto elements() const {
        for (const auto& elem : m_storage) {
            std::cout << elem << std::endl;
        }
    }

    bool empty() const { return m_storage.empty(); }
    void clear() { m_storage.clear(); }
    std::size_t size() const { return m_storage.size(); }

private:
    std::vector<T> m_storage;
    Compare compare;
};
