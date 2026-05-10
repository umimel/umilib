#ifndef AVLTREE_HPP
#define AVLTREE_HPP

/*depend on*/
#include "../header.hpp"
#include "../graph-template.hpp"

/*start*/
template<typename T>
struct avl_tree {
private:
    struct node {
        T key;
        int height;
        int size;
        int cnt; // duplicates count
        node *left, *right;

        node(T k)
            : key(k), height(1), size(1), cnt(1),
              left(nullptr), right(nullptr) {}
    };

    node* root = nullptr;

    int height(node* n) { return n ? n->height : 0; }
    int size(node* n) { return n ? n->size : 0; }

    void update(node* n) {
        if (!n) return;
        n->height = 1 + max(height(n->left), height(n->right));
        n->size = n->cnt + size(n->left) + size(n->right);
    }

    int balance_factor(node* n) {
        return height(n->left) - height(n->right);
    }

    node* rotate_right(node* y) {
        node* x = y->left;
        node* t2 = x->right;

        x->right = y;
        y->left = t2;

        update(y);
        update(x);

        return x;
    }

    node* rotate_left(node* x) {
        node* y = x->right;
        node* t2 = y->left;

        y->left = x;
        x->right = t2;

        update(x);
        update(y);

        return y;
    }

    node* balance(node* n) {
        update(n);
        int bf = balance_factor(n);

        if (bf > 1 && balance_factor(n->left) >= 0)
            return rotate_right(n);

        if (bf > 1 && balance_factor(n->left) < 0) {
            n->left = rotate_left(n->left);
            return rotate_right(n);
        }

        if (bf < -1 && balance_factor(n->right) <= 0)
            return rotate_left(n);

        if (bf < -1 && balance_factor(n->right) > 0) {
            n->right = rotate_right(n->right);
            return rotate_left(n);
        }

        return n;
    }

    node* insert(node* n, T key) {
        if (!n) return new node(key);

        if (key == n->key) {
            n->cnt++;
        } else if (key < n->key) {
            n->left = insert(n->left, key);
        } else {
            n->right = insert(n->right, key);
        }

        return balance(n);
    }

    node* min_node(node* n) {
        while (n->left) n = n->left;
        return n;
    }

    node* erase(node* n, T key) {
        if (!n) return nullptr;

        if (key < n->key) {
            n->left = erase(n->left, key);
        } else if (key > n->key) {
            n->right = erase(n->right, key);
        } else {
            if (n->cnt > 1) {
                n->cnt--;
            } else {
                if (!n->left || !n->right) {
                    node* tmp = n->left ? n->left : n->right;
                    delete n;
                    return tmp;
                } else {
                    node* tmp = min_node(n->right);
                    n->key = tmp->key;
                    n->cnt = tmp->cnt;
                    tmp->cnt = 1;
                    n->right = erase(n->right, tmp->key);
                }
            }
        }

        return balance(n);
    }

    T kth(node* n, int k) {
        if (!n) throw;
        if (k < size(n->left))
            return kth(n->left, k);
        k -= size(n->left);
        if (k < n->cnt)
            return n->key;
        return kth(n->right, k - n->cnt);
    }

    int count_less(node* n, T key) {
        if (!n) return 0;
        if (key <= n->key)
            return count_less(n->left, key);
        return size(n->left) + n->cnt +
               count_less(n->right, key);
    }

public:
    void insert(T key) {
        root = insert(root, key);
    }

    void erase(T key) {
        root = erase(root, key);
    }

    bool contains(T key) {
        node* cur = root;
        while (cur) {
            if (key == cur->key) return true;
            if (key < cur->key) cur = cur->left;
            else cur = cur->right;
        }
        return false;
    }

    int size() {
        return size(root);
    }

    T kth(int k) {
        return kth(root, k);
    }

    int count_less(T key) {
        return count_less(root, key);
    }
};
#endif