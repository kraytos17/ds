#pragma once

#include <cstdint>

template<typename T>
class RbTree {
public:
    enum Color : uint8_t {
        Red = 0,
        Black = 1,
    };

    struct Node {
        T data{};
        Node* left{nullptr};
        Node* right{nullptr};
        Color color{Black};
    };

private:
    Node* m_root{nullptr};
};
