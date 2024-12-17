#pragma once

#include <concepts>
#include <cstddef>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <utility>

template<typename T>
class CLL {
private:
    struct Node {
        T data;
        Node* next;

        template<typename U>
        explicit Node(U&& val) noexcept(std::is_nothrow_constructible_v<T, U&&>) :
            data(std::forward<U>(val)), next(nullptr) {}
    };

public:
    template<bool IsConst>
    class Iterator {
    public:
        using value_type = std::conditional_t<IsConst, const T, T>;
        using pointer = value_type*;
        using reference = value_type&;
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;

        Iterator() = default;

        template<bool WasConst>
            requires(!WasConst && IsConst)
        Iterator(const Iterator<WasConst>& other) noexcept : m_current(other.m_current), m_head(other.m_head) {}

        reference operator*() const noexcept { return m_current->data; }
        pointer operator->() const noexcept { return &m_current->data; }

        Iterator& operator++() noexcept {
            if (m_current) {
                m_current = m_current->next;
                if (m_current == m_head) {
                    m_current = nullptr;
                }
            }
            return *this;
        }

        Iterator operator++(int) noexcept {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend constexpr bool operator==(const Iterator& a, const Iterator& b) noexcept {
            return a.m_current == b.m_current;
        }

        friend constexpr bool operator!=(const Iterator& a, const Iterator& b) noexcept { return !(a == b); }

    private:
        explicit Iterator(Node* current, Node* head) noexcept : m_current(current), m_head(head) {}

        Node* m_current{nullptr};
        Node* m_head{nullptr};

        template<typename>
        friend class MyCll;
    };

    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    iterator begin() noexcept { return iterator(m_head, m_head); }
    iterator end() noexcept { return iterator(); }
    const_iterator begin() const noexcept { return const_iterator(m_head, m_head); }
    const_iterator end() const noexcept { return const_iterator(); }

    CLL() = default;

    CLL(std::initializer_list<T> init) {
        for (const auto& val: init) {
            pushBack(val);
        }
    }

    CLL(const CLL& other) { *this = other; }

    CLL(CLL&& other) noexcept = default;

    CLL& operator=(const CLL& other) {
        if (this != &other) {
            clear();
            for (const auto& val: other) {
                pushBack(val);
            }
        }
        return *this;
    }

    CLL& operator=(CLL&& other) noexcept = default;

    ~CLL() { clear(); }

    T& front() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return m_head->data;
    }

    [[nodiscard]] const T& front() const {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return m_head->data;
    }

    T& back() {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return m_tail->data;
    }

    [[nodiscard]] const T& back() const {
        if (empty()) {
            throw std::out_of_range("List is empty");
        }
        return m_tail->data;
    }

    template<std::constructible_from<T> U>
    void pushFront(U&& value) noexcept(std::is_nothrow_constructible_v<T, U&&>) {
        Node* newNode = new Node(std::forward<U>(value));

        if (!m_head) {
            m_head = newNode;
            m_tail = m_head;
            m_tail->next = m_head;
        } else {
            newNode->next = m_head;
            m_head = newNode;
            m_tail->next = m_head;
        }
        ++m_size;
    }

    template<std::constructible_from<T> U>
    void pushBack(U&& value) noexcept(std::is_nothrow_constructible_v<T, U&&>) {
        Node* newNode = new Node(std::forward<U>(value));
        if (!m_head) {
            m_head = newNode;
            m_tail = m_head;
            m_tail->next = m_head;
        } else {
            m_tail->next = newNode;
            m_tail = newNode;
            m_tail->next = m_head;
        }
        ++m_size;
    }

    void popFront() {
        if (empty()) {
            throw std::out_of_range("Cannot pop from an empty list");
        }

        if (m_size == 1) {
            delete m_head;
            m_head = m_tail = nullptr;
        } else {
            Node* temp = m_head;
            m_head = m_head->next;
            m_tail->next = m_head;
            delete temp;
        }
        --m_size;
    }

    void popBack() {
        if (empty())
            throw std::out_of_range("List is empty");
        if (m_size == 1) {
            delete m_head;
            m_head = m_tail = nullptr;
        } else {
            Node* curr = m_head;
            while (curr->next != m_tail)
                curr = curr->next;

            curr->next = m_head;
            delete m_tail;
            m_tail = curr;
        }
        --m_size;
    }

    void remove(const T& value) {
        if (empty()) {
            return;
        }

        Node* prev = m_tail;
        Node* curr = m_head;

        for (size_t i = 0; i < m_size; ++i) {
            if (curr->data == value) {
                if (curr == m_head)
                    popFront();
                else if (curr == m_tail)
                    popBack();
                else {
                    prev->next = curr->next;
                    delete curr;
                    --m_size;
                }
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }

    void print(std::ostream& os) const noexcept {
        if (empty()) {
            os << "List is empty\n";
            return;
        }
        Node* curr = m_head;
        for (size_t i = 0; i < m_size; ++i) {
            os << curr->data << " -> ";
            curr = curr->next;
        }
        os << "HEAD (circular)\n";
    }

    [[nodiscard]] constexpr bool empty() const noexcept { return m_size == 0; }
    [[nodiscard]] constexpr std::size_t size() const noexcept { return m_size; }

    void clear() noexcept {
        if (empty()) {
            return;
        }

        while (m_size > 0) {
            Node* temp = m_head;
            m_tail->next = temp->next;
            m_head = temp->next;
            delete temp;
            --m_size;
        }

        m_head = nullptr;
        m_tail = nullptr;
    }

private:
    Node* m_head{nullptr};
    Node* m_tail{nullptr};
    std::size_t m_size{0};
};
