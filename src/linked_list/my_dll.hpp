#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>

template<typename T>
class DLL {
public:
    struct Node {
        std::unique_ptr<Node> next;
        T data;

        explicit Node(T&& value, std::unique_ptr<Node> nextNode = nullptr) :
            next(std::move(nextNode)), data(std::move(value)) {}

        explicit Node(const T& value, std::unique_ptr<Node> nextNode = nullptr) :
            next(std::move(nextNode)), data(value) {}
    };

    constexpr DLL() = default;

    DLL(const DLL& other)
    requires std::copyable<T> {
        for (const auto& item: other) {
            insertBack(item);
        }
    }

    constexpr DLL(DLL&&) noexcept = default;

    DLL& operator=(const DLL& other)
        requires std::copyable<T>
    {
        if (this != &other) {
            clear();
            for (const auto& item: other) {
                insertBack(item);
            }
        }
        return *this;
    }

    constexpr DLL& operator=(DLL&&) noexcept = default;

    template<typename U = T>
        requires std::convertible_to<U, T>
    constexpr void insertFront(U&& value) {
        m_head = std::make_unique<Node>(std::forward<U>(value), std::move(m_head));
        if (!m_tail) {
            m_tail = m_head.get();
        }
    }

    template<typename U = T>
        requires std::convertible_to<U, T>
    constexpr void insertBack(U&& value) {
        if (!m_head) {
            m_head = std::make_unique<Node>(std::forward<U>(value));
            m_tail = m_head.get();
        } else {
            m_tail->next = std::make_unique<Node>(std::forward<U>(value));
            m_tail = m_tail->next.get();
        }
    }

    template<typename U = T>
        requires std::convertible_to<U, T>
    constexpr void insertAt(std::size_t idx, U&& value) {
        if (idx > count()) {
            throw std::out_of_range("Index is out of range");
        }
        if (idx == 0) {
            insertFront(std::forward<U>(value));
        } else {
            Node* curr = getNodeAt(idx - 1);
            curr->next = std::make_unique<Node>(std::forward<U>(value), std::move(curr->next));
            if (curr->next->next == nullptr) {
                m_tail = curr->next.get();
            }
        }
    }

    constexpr void removeFront() {
        if (m_head) {
            m_head = std::move(m_head->next);
            if (!m_head) {
                m_tail = nullptr;
            }
        } else {
            throw std::underflow_error("Linked list is empty");
        }
    }

    constexpr void removeBack() {
        if (!m_head) {
            throw std::underflow_error("Linked list is empty");
        }

        if (!m_head->next) {
            m_head.reset();
            m_tail = nullptr;
        } else {
            Node* curr = m_head.get();
            while (curr->next && curr->next->next) {
                curr = curr->next.get();
            }
            curr->next.reset();
            m_tail = curr;
        }
    }

    constexpr void removeAt(std::size_t idx) {
        if (idx >= count()) {
            throw std::out_of_range("Index is out of range");
        }
        if (idx == 0) {
            removeFront();
        } else {
            Node* curr = getNodeAt(idx - 1);
            curr->next = std::move(curr->next->next);
            if (!curr->next) {
                m_tail = curr;
            }
        }
    }

    [[nodiscard]] constexpr bool find(const T& value) const noexcept {
        return std::ranges::any_of(begin(), end(), [&value](const T& item) { return item == value; });
    }

    [[nodiscard]] constexpr std::optional<T> findIf(std::invocable<const T&> auto predicate) const noexcept {
        for (const auto& item: *this) {
            if (std::invoke(predicate, item)) {
                return item;
            }
        }
        return std::nullopt;
    }

    constexpr void reverse() noexcept {
        std::unique_ptr<Node> prev;
        auto curr = std::move(m_head);
        m_tail = curr.get();
        while (curr) {
            auto nextNode = std::move(curr->next);
            curr->next = std::move(prev);
            prev = std::move(curr);
            curr = std::move(nextNode);
        }
        m_head = std::move(prev);
    }

    [[nodiscard]] constexpr const T& getAt(std::size_t idx) const {
        if (idx >= count()) {
            throw std::out_of_range("Index is out of range");
        }
        return getNodeAt(idx)->data;
    }

    [[nodiscard]] constexpr std::size_t count() const noexcept { return std::ranges::distance(begin(), end()); }

    constexpr void clear() noexcept {
        m_head.reset();
        m_tail = nullptr;
    }

    template<typename Self>
    constexpr void print(this Self&& self) noexcept {
        for (const auto& item: self) {
            std::cout << item << " ";
        }
    }

    [[nodiscard]] constexpr bool isEmpty() const noexcept { return !m_head; }

    template<bool IsConst>
    class Iterator {
    public:
        using iterator_concept = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = std::conditional_t<IsConst, const T*, T*>;
        using reference = std::conditional_t<IsConst, const T&, T&>;

        constexpr Iterator() noexcept = default;
        explicit constexpr Iterator(Node* node) noexcept : m_current(node) {}

        constexpr reference operator*() const { return m_current->data; }
        constexpr pointer operator->() const { return &m_current->data; }

        constexpr Iterator& operator++() noexcept {
            m_current = m_current->next.get();
            return *this;
        }

        constexpr Iterator operator++(int) noexcept {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        constexpr bool operator==(const Iterator& b) noexcept { return m_current == b.m_current; }

    private:
        Node* m_current{nullptr};
    };

    template<typename Self>
    [[nodiscard]] constexpr auto begin(this Self&& self) noexcept {
        return Iterator(self.m_head.get());
    }

    template<typename Self>
    [[nodiscard]] constexpr auto end(this Self&&) noexcept {
        return Iterator(nullptr);
    }

private:
    std::unique_ptr<Node> m_head;
    Node* m_tail{nullptr};

    Node* getNodeAt(std::size_t idx) const {
        Node* curr = m_head.get();
        for (std::size_t i{0}; i < idx; ++i) {
            curr = curr->next.get();
        }
        return curr;
    }
};
