#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <stdexcept>
#include <utility>

template<typename T>
    requires std::totally_ordered<T>
class BST {
public:
    struct Node {
        T data{};
        Node* left{nullptr};
        Node* right{nullptr};

        template<typename U>
        constexpr explicit Node(U&& value) : data{std::forward<U>(value)}, left{}, right{} {}
    };

    constexpr BST() = default;

    constexpr BST(Node* root) : m_root{root} {}

    void insert(T value) {
        Node* newNode = new Node{value};
        if (!m_root) {
            m_root = newNode;
            return;
        }

        Node* curr{m_root};
        Node* par = nullptr;

        while (curr) {
            par = curr;
            if (value < curr->data) {
                curr = curr->left;
            } else if (value > curr->data) {
                curr = curr->right;
            } else {
                // ignore duplicates.
                delete newNode;
                return;
            }
        }

        if (value < par->data) {
            par->left = newNode;
        }

        if (value > par->data) {
            par->right = newNode;
        }
    }

    Node* remove(Node* root, T value) {
        if (!root) {
            return root;
        }

        Node* curr{m_root};
        Node* par = nullptr;

        while (curr && curr->data != value) {
            par = curr;
            if (value < curr->data) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }

            if (!curr) {
                return root;
            }

            if (!curr->left && !curr->right) {
                deleteNode(par, curr, nullptr);
            } else if (!curr->left && curr->right) {
                deleteNode(par, curr, curr->right);
            } else if (curr->left && !curr->right) {
                deleteNode(par, curr, curr->left);
            } else {
                Node* successorParent = curr;
                Node* successor = curr->right;
                while (successor->left) {
                    successorParent = successor;
                    successor = successor->left;
                }

                curr->data = successor->data;
                if (successor->right) {
                    deleteNode(successorParent, successor, successor->right);
                } else {
                    deleteNode(successorParent, successor, nullptr);
                }
            }
        }

        return root;
    }

    bool search(const T& value) const {
        Node* curr{m_root};
        while (curr) {
            if (value == m_root->data) {
                return true;
            } else if (value < m_root->data) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }

        return false;
    }

    const T min() const {
        if (!m_root) {
            throw std::runtime_error("The tree is empty, no min value.");
        }

        Node* curr{m_root};
        while (curr->left) {
            curr = curr->left;
        }
        return curr->data;
    }

    const T max() const {
        if (!m_root) {
            throw std::runtime_error("The tree is empty, no max value.");
        }

        Node* curr{m_root};
        while (curr->right) {
            curr = curr->right;
        }
        return curr->data;
    }

    std::size_t height() const {
        Node* curr{m_root};
        if (!curr) {
            return 0;
        }
        return 1 + std::max(height(curr->left), height(curr->right));
    }

    bool isBalanced() const {
        Node* curr{m_root};
        if (!curr) {
            return true;
        }

        int lh = height(curr->left);
        int rh = height(curr->right);
        return lh - rh <= 1 && isBalanced(curr->left) && isBalanced(curr->right);
    }

    const T successor(const T& value) const {
        Node* curr{m_root};
        Node* succ{nullptr};

        while (curr) {
            if (value < curr->data) {
                succ = curr;
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }

        if (!succ) {
            throw std::runtime_error("No successor found.");
        }
        return succ->data;
    }

private:
    Node* m_root{nullptr};

    void deleteNode(Node* parent, Node* toDel, Node* child) {
        if (!parent) {
            m_root = child;
        } else if (parent->left == toDel) {
            parent->left = child;
        } else {
            parent->right = child;
        }

        delete toDel;
    }
};
