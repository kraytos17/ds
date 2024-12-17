#pragma once

#include <concepts>
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

        Node* curr = m_root;
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

        Node* curr = m_root;
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
