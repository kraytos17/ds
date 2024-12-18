#pragma once

#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

template<typename T, typename Alloc = std::allocator<T>>
class Vector {
public:
    Vector() : m_data{nullptr}, m_size{0}, m_cap{0} {}

    Vector(std::size_t count, const T& elem = T{}) {
        allocateMemory(count);
        constructElements(0, count, elem);
        m_size = m_cap = count;
    }

    Vector(const Vector& other) : m_data{nullptr}, m_size{0}, m_cap{0} {
        allocateMemory(other.m_cap);
        constructElementsFrom(0, other.m_data, other.m_size);
        m_size = other.m_size;
    }

    Vector& operator=(Vector other) noexcept {
        swap(*this, other);
        return *this;
    }

    Vector(Vector&& other) noexcept : Vector() { swap(*this, other); }

    Vector& operator=(Vector&& other) noexcept {
        swap(*this, other);
        return *this;
    }

    Vector(std::initializer_list<T> init) {
        allocateMemory(init.size());
        constructElementsFrom(0, init.begin(), init.size());
        m_size = m_cap = init.size();
    }

    ~Vector() {
        destroyElements(0, m_size);
        deallocateMemory();
    }

    constexpr auto operator<=>(const Vector& other) const noexcept = default;

    [[nodiscard]] constexpr T& operator[](std::size_t idx) { return m_data[idx]; }
    [[nodiscard]] constexpr const T& operator[](std::size_t idx) const { return m_data[idx]; }

    constexpr T& at(std::size_t idx) {
        if (idx >= m_size)
            throw std::out_of_range{"Index out of range"};
        return m_data[idx];
    }

    constexpr const T& at(std::size_t idx) const {
        if (idx >= m_size)
            throw std::out_of_range{"Index out of range"};
        return m_data[idx];
    }

    [[nodiscard]] constexpr T& front() { return m_data[0]; }
    [[nodiscard]] constexpr const T& front() const { return m_data[0]; }
    [[nodiscard]] constexpr T& back() { return m_data[m_size - 1]; }
    [[nodiscard]] constexpr const T& back() const { return m_data[m_size - 1]; }
    [[nodiscard]] constexpr T* data() noexcept { return m_data; }
    [[nodiscard]] constexpr const T* data() const noexcept { return m_data; }
    [[nodiscard]] constexpr std::size_t size() const noexcept { return m_size; }
    [[nodiscard]] constexpr std::size_t capacity() const noexcept { return m_cap; }
    [[nodiscard]] constexpr bool empty() const noexcept { return m_size == 0; }

    void reserve(std::size_t newCap) {
        if (newCap > m_cap) {
            reallocate(newCap);
        }
    }

    void shrinkToFit() {
        if (m_size < m_cap) {
            reallocate(m_size);
        }
    }

    void resize(std::size_t newSize, const T& elem = T{}) {
        if (newSize < m_size) {
            destroyElements(newSize, m_size);
        } else if (newSize > m_size) {
            if (newSize > m_cap) {
                reallocate(newSize);
            }
            constructElements(m_size, newSize, elem);
        }

        m_size = newSize;
    }

    void clear() noexcept {
        destroyElements(0, m_size);
        m_size = 0;
    }

    void pushBack(const T& value) {
        if (m_size == m_cap) {
            grow();
        }
        ::new (m_data + m_size) T{value};
        ++m_size;
    }

    template<typename... Args>
    void emplaceBack(Args&&... args) {
        if (m_size == m_cap) {
            grow();
        }
        try {
            ::new (m_data + m_size) T{std::forward<Args>(args)...};
            ++m_size;
        } catch (...) {
            --m_size;
            throw;
        }
    }

    constexpr void popBack() {
        if (m_size > 0) {
            m_data[m_size - 1].~T();
            --m_size;
        }
    }

    constexpr void erase(const T& value) {
        auto newEnd = std::remove(begin(), end(), value);
        destroyElements(newEnd - begin(), m_size);
        m_size = newEnd - begin();
    }

    void assign(std::size_t count, const T& value) {
        clear();
        if (count > m_cap) {
            reallocate(count);
        }
        constructElements(0, count, value);
        m_size = count;
    }

    void assign(std::initializer_list<T> init) { assign(init.size(), init.begin(), init.end()); }

    using iterator = T*;
    using const_iterator = const T*;

    [[nodiscard]] iterator begin() noexcept { return m_data; }
    [[nodiscard]] const_iterator begin() const noexcept { return m_data; }
    [[nodiscard]] iterator end() noexcept { return m_data + m_size; }
    [[nodiscard]] const_iterator end() const noexcept { return m_data + m_size; }
    [[nodiscard]] const_iterator cbegin() const noexcept { return m_data; }
    [[nodiscard]] const_iterator cend() const noexcept { return m_data + m_size; }

    friend void swap(Vector& first, Vector& second) noexcept {
        using std::swap;
        swap(first.m_data, second.m_data);
        swap(first.m_size, second.m_size);
        swap(first.m_cap, second.m_cap);
    }

private:
    T* m_data{};
    std::size_t m_size{};
    std::size_t m_cap{};
    Alloc m_allocator{};

    void allocateMemory(std::size_t count) {
        m_data = m_allocator.allocate(count);
        m_cap = count;
    }

    void deallocateMemory() {
        if (m_data) {
            m_allocator.deallocate(m_data, m_cap);
            m_data = nullptr;
            m_cap = 0;
        }
    }

    void constructElements(std::size_t start, std::size_t end, const T& elem) {
        for (std::size_t i = start; i < end; ++i) {
            ::new (m_data + i) T{elem};
        }
    }

    template<typename InputIt>
    void constructElementsFrom(std::size_t start, InputIt first, std::size_t count) {
        for (std::size_t i = 0; i < count; ++i) {
            ::new (m_data + start + i) T{*(first + i)};
        }
    }

    void destroyElements(std::size_t start, std::size_t end) {
        for (std::size_t i = start; i < end; ++i) {
            m_data[i].~T();
        }
    }

    void reallocate(std::size_t newCap) {
        T* newData = m_allocator.allocate(newCap);

        if constexpr (std::is_nothrow_move_constructible_v<T>) {
            for (std::size_t i = 0; i < m_size; ++i) {
                ::new (newData + i) T(std::move(m_data[i]));
                m_data[i].~T();
            }
        } else {
            for (std::size_t i = 0; i < m_size; ++i) {
                ::new (newData + i) T(m_data[i]);
                m_data[i].~T();
            }
        }

        deallocateMemory();
        m_data = newData;
        m_cap = newCap;
    }

    void grow() {
        std::size_t newCap = (m_cap == 0) ? 4 : m_cap * 2;
        reallocate(newCap);
    }
};
