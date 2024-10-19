#pragma once

#include <algorithm>
#include <memory>
#include <optional>
#include "adt.hpp"

template<typename T>
class AList final : public IList<T> {
public:
    static constexpr int64 defaultSize{10};

    constexpr explicit AList(int64 size = defaultSize) noexcept :
        m_listArr(std::make_unique<T[]>(size)), m_maxSize(size), m_listSize(0), m_curr(0) {}

    constexpr void clear() noexcept override { m_listSize = m_curr = 0; }

    constexpr bool insert(const T& it) noexcept override {
        if (m_listSize >= m_maxSize) {
            resize(m_maxSize * 2);
        }

        std::shift_right(m_listArr.get() + m_curr, m_listArr.get() + m_listSize, 1);
        m_listArr[m_curr] = it;
        ++m_listSize;

        return true;
    }

    constexpr bool append(const T& it) noexcept override {
        if (m_listSize >= m_maxSize) {
            resize(m_maxSize * 2);
        }
        m_listArr[m_listSize++] = it;

        return true;
    }

    constexpr std::optional<T> remove() noexcept override {
        if (m_curr < 0 || m_curr >= m_listSize) {
            return std::nullopt;
        }

        T it = std::move(m_listArr[m_curr]);
        std::shift_left(m_listArr.get() + m_curr, m_listArr.get() + m_listSize, 1);
        --m_listSize;

        return it;
    }

    constexpr void moveToStart() noexcept override { m_curr = 0; }
    constexpr void moveToEnd() noexcept override { m_curr = m_listSize; }
    constexpr void prev() noexcept override {
        if (m_curr != 0) {
            --m_curr;
        }
    }

    constexpr void next() noexcept override {
        if (m_curr < m_listSize) {
            ++m_curr;
        }
    }

    [[nodiscard]] constexpr int64 length() const noexcept override { return m_listSize; }
    [[nodiscard]] constexpr int64 currPos() const noexcept override { return m_curr; }

    constexpr bool moveToPos(int64 pos) noexcept override {
        if (pos < 0 || pos > m_listSize) {
            return false;
        }

        m_curr = pos;
        return true;
    }

    [[nodiscard]] constexpr bool isAtEnd() const noexcept override { return m_curr == m_listSize; }
    [[nodiscard]] constexpr std::optional<T&> getValue() const noexcept override {
        if (m_curr < 0 || m_curr >= m_listSize) {
            return std::nullopt;
        }

        return m_listArr[m_curr];
    }

    [[nodiscard]] constexpr bool isEmpty() const noexcept override { return m_listSize == 0; }

private:
    std::unique_ptr<T[]> m_listArr;
    int64 m_maxSize{0};
    int64 m_listSize{0};
    int64 m_curr{0};

    constexpr void resize(int64 newSize) noexcept {
        std::unique_ptr<T[]> newArr = std::make_unique<T[]>(newSize);
        std::move(m_listArr.get(), m_listArr.get() + m_listSize, newArr.get());
        m_listArr = std::move(newArr);
        m_maxSize = newSize;
    }
};
