#pragma once

#include <cstddef>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

template<typename T>
class CLL {
private:
    struct Node {
        T data{};
        Node* next{};

        template<typename U>
        explicit Node(U&& val) noexcept : data(std::forward<U>(val)), next{nullptr} {}
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

        explicit Iterator() = default;

        explicit Iterator(std::conditional_t<IsConst, const Node*, Node*> head,
                          std::conditional_t<IsConst, const Node*, Node*> current) noexcept :
            m_current(current), m_head(head) {}

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
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator& rhs) const noexcept { return m_current == rhs.m_current; }
        bool operator!=(const Iterator& rhs) const noexcept { return !(*this == rhs); }

    private:
        std::conditional_t<IsConst, const Node*, Node*> m_current{};
        std::conditional_t<IsConst, const Node*, Node*> m_head{};
    };

public:
    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    auto begin(this auto& self) noexcept {
        using IterType =
            std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const_iterator, iterator>;
        return IterType(self.m_head, self.m_head);
    }

    auto end(this auto& self) noexcept {
        using IterType =
            std::conditional_t<std::is_const_v<std::remove_reference_t<decltype(self)>>, const_iterator, iterator>;
        return IterType();
    }

    CLL() = default;

    CLL(std::initializer_list<T> init) {
        for (const auto& val: init) {
            pushBack(val);
        }
    }

    explicit CLL(const CLL& other) { *this = other; }

    explicit CLL(CLL&& other) noexcept = default;

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

    auto& front(this auto& self) {
        if (self.empty()) {
            throw std::out_of_range("List is empty");
        }
        return self.m_head->data;
    }

    auto& back(this auto& self) {
        if (self.empty()) {
            throw std::out_of_range("List is empty");
        }
        return self.m_tail->data;
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
    size_t m_size{0};
};
