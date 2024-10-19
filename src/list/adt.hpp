#pragma once

#include <cstddef>

template<typename T>
class IList {
public:
    virtual ~IList() = default;

    virtual void clear() noexcept = 0;
    virtual bool insert(const T& it) noexcept = 0;
    virtual bool append(const T& it) noexcept = 0;
    virtual T remove() noexcept = 0;
    virtual void moveToStart() noexcept = 0;
    virtual void moveToEnd() noexcept = 0;
    virtual void prev() noexcept = 0;
    virtual void next() noexcept = 0;
    virtual bool moveToPos(std::ptrdiff_t pos) noexcept = 0;

    [[nodiscard]] virtual std::ptrdiff_t length() const noexcept = 0;
    [[nodiscard]] virtual std::ptrdiff_t currPos() const noexcept = 0;
    [[nodiscard]] virtual bool isAtEnd() const noexcept = 0;
    [[nodiscard]] virtual const T& getValue() const noexcept = 0;
    [[nodiscard]] virtual bool isEmpty() const noexcept = 0;
};
