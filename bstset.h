#ifndef BSTSET
#define BSTSET

#include <iostream>

template <typename T>
class BSTSet {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(T value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // Recursive insert function
    Node* insert(Node* node, const T& value) {
        if (!node) return new Node(value);
        if (value < node->data) {
            node->left = insert(node->left, value);
        }
        else if (node->data < value ) {
            node->right = insert(node->right, value);
        }
        else if (node->data == value ){
            node->data = value; // Replace existing value
        }
        return node;
    }

    // Recursive search function
    Node* find(Node* node, const T& value) const {
        if (!node) return nullptr;
        if (value < node->data) return find(node->left, value);
        if (value > node->data) return find(node->right, value);
        return node; // Found
    }

    // Find the smallest node >= value
    Node* find_first_not_smaller(Node* node, const T& value) const {
        Node* res = nullptr;
        while (node) {
            if (!(node->data < value)) {
                res = node;
                node = node->left; // Try finding a smaller valid node
            }
            else {
                node = node->right;
            }
        }
        return res;
    }

    // Find the leftmost node
    Node* findMin(Node* node) const {
        while (node && node->left) node = node->left;
        return node;
    }

    // Delete all nodes (for destructor)
    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    class SetIterator;
    BSTSet() : root(nullptr) {}

    ~BSTSet() { destroyTree(root); }

    void insert(const T& value) {
        root = insert(root, value);
    }

    SetIterator find(const T& value) const {
        return SetIterator(*this, find(root, value));
    }

    SetIterator find_first_not_smaller(const T& value) const {
        return SetIterator(*this, find_first_not_smaller(root, value));
    }

    class SetIterator {
    private:
        Node* current;
        const BSTSet<T>& bst; // Reference to the BSTSet instance

        Node* nextLarger(Node* node) {
            if (!node) return nullptr;

            if (node->right) {
                return bst.findMin(node->right);
            }

            Node* succ = nullptr;
            Node* ancestor = bst.root;  

            while (ancestor) {
                if (node->data < ancestor->data) {
                    succ = ancestor; 
                    ancestor = ancestor->left;
                }
                else if (ancestor->data < node->data) {
                    ancestor = ancestor->right; 
                }
                else {
                    break; 
                }
            }

            return succ; 
        }


    public:
        SetIterator(const BSTSet<T>& bstSet, Node* node) : bst(bstSet), current(node) {}

        const T* get_and_advance() {
            if (!current) {
                return nullptr;
            }
            const T* result = &(current->data);
            current = nextLarger(current);
            return result;
        }
    };
};


#endif // !BSTSET