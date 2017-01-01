#ifndef BINARY_TREE_H
#define BINARY_TREE_H

template<typename T>
class BinaryTree {
public:
    BinaryTree() : root(nullptr) {}
    ~BinaryTree()
    {
        delete root;
    }
    bool empty() const
    {
        return root == nullptr;
    }
    void remove(const T& val)
    {
        node_remove(root, val);
    }
    void insert(const T& val)
    {
        auto tmp = node_insert(root, val);
        if (root == nullptr) {
            root = tmp;
        }
    }
    bool search(const T& val)
    {
        return node_search(root, val) != nullptr;
    }
    template<typename F>
    void foreach(F fn)
    {
        visit(root, fn);
    }
private:
    struct BinaryNode {
        BinaryNode* parent;
        BinaryNode* left;
        BinaryNode* right;
        T key;
        BinaryNode(const T& val) : key(val), left(nullptr), right(nullptr) {}
        ~BinaryNode()
        {
            delete left;
            delete right;
        }
    };
    template<typename F>
    void visit(BinaryNode* node, F fn)
    {
        if (node->left != nullptr) {
            visit(node->left, fn);
        }
        fn(node->key);
        if (node->right != nullptr) {
            visit(node->right, fn);
        }
    }
    BinaryNode* tree_minimum(BinaryNode* node)
    {
        BinaryNode* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }
    BinaryNode* node_search(BinaryNode* node, const T& val)
    {
        BinaryNode* current = node;
        while (current != nullptr && current->key != val) {
            if (current->key > val) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        return current;
    }
    BinaryNode* node_insert(BinaryNode* node, const T& val)
    {
        if (node == nullptr) {
            return new BinaryNode {val};
        }
        BinaryNode* current = node;
        BinaryNode** to_insert = nullptr;
        BinaryNode* pre = nullptr;
        while (current != nullptr) {
            if (current->key > val) {
                pre = current;
                to_insert = &(current->left);
                current = current->left;
            } else if (current->key < val) {
                pre = current;
                to_insert = &(current->right);
                current = current->right;
            } else {
                return current;
            }
        }
        *to_insert = new BinaryNode {val};
        (*to_insert)->parent = pre;
        return *to_insert;
    }
    BinaryNode** get_which_child(BinaryNode* child)
    {
        if (child == nullptr || child->parent == nullptr) {
            return nullptr;
        } else if (child->parent->left == child) {
            return &(child->parent->left);
        } else if (child->parent->right == child) {
            return &(child->parent->right);
        } else {
            return nullptr;
        }
    }
    void node_destroy(BinaryNode* dest)
    {
        if (dest != nullptr) {
            dest->left = dest->right = nullptr;
            delete dest;
        }
    }
    void node_remove(BinaryNode* node, const T& val)
    {
        BinaryNode* tmp = nullptr;
        BinaryNode* dest = node_search(node, val);
        if (dest == nullptr) {
            return;
        } else if (dest->left == nullptr && dest->right == nullptr) {
            if (dest->parent == nullptr) {
                root = nullptr;
                node_destroy(dest);
            } else {
                *(get_which_child(dest)) = nullptr;
                node_destroy(dest);
            }
        } else if (dest->left != nullptr && dest->right == nullptr) {
            if (dest->parent == nullptr) { // delete root
                tmp = dest->left;
                tmp->parent = nullptr;
                node_destroy(dest);
                root = tmp;
            } else {
                *(get_which_child(dest)) = dest->left;
                dest->left->parent = dest->parent;
                node_destroy(dest);
            }
        } else if (dest->left == nullptr && dest->right != nullptr) {
            if (dest->parent == nullptr) {
                tmp = dest->right;
                tmp->parent = nullptr;
                node_destroy(dest);
                root = tmp;
            } else {
                *(get_which_child(dest)) = dest->right;
                dest->right->parent = dest->parent;
                node_destroy(dest);
            }
        } else { // both child-trees are not empty
            tmp = tree_minimum(dest->right);
            if (tmp->parent == dest) {
                tmp->left = dest->left;
                dest->left->parent = tmp;
                tmp->parent = dest->parent;
                if (dest->parent == nullptr) {
                    node_destroy(dest);
                    root = tmp;
                } else {
                    *(get_which_child(dest)) = tmp;
                    node_destroy(dest);
                }
            } else {
                // most complex case
                tmp->parent->left = tmp->right;
                if (tmp->right != nullptr) {
                    tmp->right->parent = tmp->parent;
                }
                tmp->left = dest->left;
                dest->left->parent = tmp;
                tmp->right = dest->right;
                dest->right->parent = tmp;
                tmp->parent = dest->parent;
                if (dest->parent == nullptr) {
                    node_destroy(dest);
                    root = tmp;
                } else {
                    *(get_which_child(dest)) = tmp;
                    node_destroy(dest);
                }
            }
        }
    }
    BinaryNode* root;
};

#endif // BINARY_TREE_H