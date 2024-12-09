#pragma once

#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <type_traits>

template<typename T, typename Alloc = std::allocator<T>>
class MyVec {
public:
    MyVec() : m_data{nullptr}, m_size{0}, m_cap{0} {}

    MyVec(std::size_t count, const T& elem = T{}) {
        m_data = m_allocator.allocate(count);
        for (std::size_t i{0}; i < count; ++i) {
            ::new (static_cast<void*>(m_data + i)) T{elem};
        }
        m_size = m_cap = count;
    }

    MyVec(const MyVec& other) : m_data{nullptr}, m_size{other.m_size}, m_cap{other.m_cap} {
        if (m_cap > 0) {
            m_data = m_allocator.allocate(m_cap);
            for (std::size_t i{0}; i < m_size; ++i) {
                ::new (static_cast<void*>(m_data + i)) T{other.m_data[i]};
            }
        }
    }

    MyVec& operator=(const MyVec& other) {
        if (this == &other) {
            return *this;
        }

        clear();
        if (m_cap < other.m_size) {
            if (m_data) {
                m_allocator.deallocate(m_data, m_cap);
            }
            m_cap = other.m_cap;
            m_data = m_allocator.allocate(m_cap);
        }

        m_size = other.m_size;
        for (std::size_t i{0}; i < m_size; ++i) {
            ::new (static_cast<void*>(m_data + i)) T{other.m_data[i]};
        }

        return *this;
    }

    MyVec(MyVec&& other) noexcept : m_data{other.m_data}, m_size{other.m_size}, m_cap{other.m_cap} {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_cap = 0;
    }

    MyVec& operator=(MyVec&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        clear();
        if (m_data) {
            m_allocator.deallocate(m_data, m_cap);
        }
        m_data = other.m_data;
        m_size = other.m_size;
        m_cap = other.m_cap;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_cap = 0;

        return *this;
    }

    MyVec(std::initializer_list<T> init) : m_data{nullptr}, m_size{init.size()}, m_cap{init.size()} {
        if (m_cap > 0) {
            m_data = m_allocator.allocate(m_cap);
            std::size_t i{0};
            for (const T& val: init) {
                ::new (static_cast<void*>(m_data + i)) T{val};
                ++i;
            }
        }
    }

    ~MyVec() {
        clear();
        if (m_data) {
            m_allocator.deallocate(m_data, m_cap);
        }
    }

    auto operator<=>(const MyVec& other) const noexcept = default;

    void clear() {
        for (std::size_t i{0}; i < m_size; ++i) {
            m_data[i].~T();
        }
        m_size = 0;
    }

    std::size_t size() const { return m_size; }
    std::size_t capacity() const { return m_cap; }
    T& operator[](std::size_t idx) { return m_data[idx]; }
    const T& operator[](std::size_t idx) const { return m_data[idx]; }

    T& at(std::size_t idx) {
        if (idx >= m_size) {
            throw std::out_of_range{"Index out of range"};
        }
        return m_data[idx];
    }

    const T& at(std::size_t idx) const {
        if (idx >= m_size) {
            throw std::out_of_range{"Index out of range"};
        }
        return m_data[idx];
    }

    T& front() { return m_data[0]; }
    const T& front() const { return m_data[0]; }
    T& back() { return m_data[m_size - 1]; }
    const T& back() const { return m_data[m_size - 1]; }
    T* data() noexcept { return m_data; }
    const T* data() const noexcept { return m_data; }
    bool empty() const noexcept { return m_size == 0; }

    void pushBack(const T& element) {
        if (m_size == m_cap) {
            grow();
        }
        ::new (static_cast<void*>(m_data + m_size)) T{element};
        ++m_size;
    }

    template<typename... Args>
    void emplaceBack(Args&&... args) {
        if (m_size == m_cap) {
            grow();
        }

        ::new (static_cast<void*>(m_data + m_size)) T{std::forward<Args>(args)...};
        ++m_size;
    }

    void popBack() {
        if (m_size > 0) {
            m_data[m_size - 1].~T();
            --m_size;
        }
    }

    void resize(std::size_t newSize, const T& elem = T{}) {
        if (newSize < m_size) {
            for (std::size_t i{newSize}; i < m_size; ++i) {
                m_data[i].~T();
            }
        } else if (newSize > m_size) {
            if (newSize > m_cap) {
                reserve(newSize);
            }
            for (std::size_t i{m_size}; i < newSize; ++i) {
                ::new (static_cast<void*>(m_data + i)) T{elem};
            }
        }
        m_size = newSize;
    }

    void reserve(std::size_t newCap) {
        if (newCap > m_cap) {
            growTo(newCap);
        }
    }

    void shrinkToFit() {
        if (m_size < m_cap) {
            T* newData = m_allocator.allocate(m_size);
            for (std::size_t i = 0; i < m_size; ++i) {
                ::new (static_cast<void*>(newData + i)) T{std::move(m_data[i])};
                m_data[i].~T();
            }
            if (m_data) {
                m_allocator.deallocate(m_data, m_cap);
            }
            m_data = newData;
            m_cap = m_size;
        }
    }

    void insert(std::size_t pos, const T& value) {
        if (pos > m_size) {
            return;
        }

        if (m_size == m_cap) {
            grow();
        }

        for (std::size_t i{m_size}; i > pos; --i) {
            ::new (static_cast<void*>(m_data + i)) T{std::move(m_data[i - 1])};
            m_data[i - 1].~T();
        }

        ::new (static_cast<void*>(m_data + pos)) T{value};
        ++m_size;
    }

    template<typename InputIt>
    void insert(std::size_t pos, InputIt first, InputIt last) {
        auto count = std::ranges::distance(first, last);
        if (count <= 0 || pos > m_size) {
            return;
        }

        while (m_size + count > m_cap) {
            grow();
        }

        for (std::size_t i{m_size + count - 1}; i >= pos + count; --i) {
            ::new (static_cast<void*>(m_data + i)) T{std::move(m_data[i - count])};
            m_data[i - count].~T();
        }

        for (auto it{first}; it != last; ++it, ++pos) {
            ::new (static_cast<void*>(m_data + pos)) T{*it};
        }

        m_size += count;
    }

    void insert(std::size_t pos, std::initializer_list<T> init) { insert(pos, init.begin(), init.end()); }

    void erase(std::size_t pos) {
        if (pos >= m_size)
            return;

        m_data[pos].~T();
        for (std::size_t i{pos}; i < m_size - 1; ++i) {
            ::new (static_cast<void*>(m_data + i)) T{std::move(m_data[i + 1])};
            m_data[i + 1].~T();
        }
        --m_size;
    }

    void erase(std::size_t first, std::size_t last) {
        if (first >= m_size || last > m_size || first >= last)
            return;

        for (std::size_t i{first}; i < last; ++i) {
            m_data[i].~T();
        }

        std::size_t range_size = last - first;
        for (std::size_t i{first}; i < m_size - range_size; ++i) {
            ::new (static_cast<void*>(m_data + i)) T{std::move(m_data[i + range_size])};
            m_data[i + range_size].~T();
        }
        m_size -= range_size;
    }

    void assign(std::size_t count, const T& value) {
        clear();
        if (count > m_cap) {
            reserve(count);
        }
        for (std::size_t i{0}; i < count; ++i) {
            ::new (static_cast<void*>(m_data + i)) T{value};
        }
        m_size = count;
    }

    void assign(std::initializer_list<T> init) { assign(init.begin(), init.end()); }

    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() noexcept { return m_data; }
    const_iterator begin() const noexcept { return m_data; }
    iterator end() noexcept { return m_data + m_size; }
    const_iterator end() const noexcept { return m_data + m_size; }

    const_iterator cbegin() const noexcept { return m_data; }
    const_iterator cend() const noexcept { return m_data + m_size; }

    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

private:
    T* m_data{};
    std::size_t m_size{};
    std::size_t m_cap{};
    std::allocator<T> m_allocator{};

    constexpr void grow() {
        std::size_t newCap = (m_cap == 0) ? 4 : m_cap * 2;
        T* newData = m_allocator.allocate(newCap);
        if constexpr (std::is_trivially_copyable_v<T>) {
            std::memcpy(newData, m_data, m_size * sizeof(T));
        } else {
            for (std::size_t i{0}; i < m_size; ++i) {
                ::new (static_cast<void*>(newData + i)) T{std::move(m_data[i])};
                m_data[i].~T();
            }
        }

        if (m_data) {
            m_data->~T();
        }

        m_data = newData;
        m_cap = newCap;
    }

    constexpr void growTo(std::size_t newCap) {
        T* newData = m_allocator.allocate(newCap);
        for (std::size_t i{0}; i < newCap; ++i) {
            ::new (static_cast<void*>(newData + i)) T{std::move(m_data[i])};
            m_data[i].~T();
        }

        if (m_data) {
            m_allocator.deallocate(m_data, m_cap);
        }
        m_data = newData;
        m_cap = newCap;
    }
};
