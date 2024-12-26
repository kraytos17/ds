#pragma once

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <iostream>
#include <queue>
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

    constexpr BST(const BST& other) { m_root = copy(other.m_root); }

    constexpr BST& operator=(const BST& other) {
        if (this != &other) {
            clear(m_root);
            m_root = copy(other.m_root);
        }
        return *this;
    }

    constexpr BST(BST&& other) noexcept : m_root(other.m_root) { other.m_root = nullptr; }

    constexpr BST& operator=(BST&& other) noexcept {
        if (this != &other) {
            clear(m_root);
            m_root = other.m_root;
            other.m_root = nullptr;
        }
        return *this;
    }


    ~BST() { clear(m_root); }

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

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

    std::size_t height() const { return height(m_root); }
    std::size_t count() const { return count(m_root); }

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

    void inorderTraversal(Node* root) {
        if (!root) {
            return;
        }

        inorderTraversal(root->left);
        std::cout << root->data << '\n';
        inorderTraversal(root->right);
    }

    void preorderTraversal(Node* root) {
        if (!root) {
            return;
        }

        std::cout << root->data << '\n';
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }

    void postorderTraversal(Node* root) {
        if (!root) {
            return;
        }

        postorderTraversal(root->left);
        postorderTraversal(root->right);
        std::cout << root->data << '\n';
    }

    void levelorderTraversal() const {
        if (!m_root) {
            return;
        }

        std::queue<Node*> q;
        while (!q.empty()) {
            Node* curr = q.front();
            q.pop();
            std::cout << curr->data << '\n';

            if (curr->left) {
                q.push(curr->left);
            }

            if (curr->right) {
                q.push(curr->right);
            }
        }

        std::cout << '\n';
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

    Node* copy(Node* node) const {
        if (!node) {
            return nullptr;
        }

        Node* newNode = new Node(node->data);
        newNode->left = copy(node->left);
        newNode->right = copy(node->right);

        return newNode;
    }

    std::size_t height(Node* root) const {
        if (!root) {
            return 0;
        }
        return 1 + std::max(height(root->left), height(root->right));
    }

    std::size_t count(Node* root) const {
        if (!root) {
            return 0;
        }
        return 1 + count(root->left) + count(root->right);
    }
};
