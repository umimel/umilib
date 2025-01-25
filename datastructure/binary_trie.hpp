#ifndef BINARY_TRIE_HPP
#define BINARY_TRIE_HPP

/*depend on*/
#include "../header.hpp"

/*start*/
template<typename U = unsigned, int B = 32>
class binary_trie{
    struct node{
        int cnt;
        node *ch[2];
        node() : cnt(0), ch{nullptr, nullptr}{}
    };
    node* add(node* t, U val, int b = B-1){
        if(!t) t = new node;
        t->cnt++;
        if(b < 0) return t;
        bool f = (val>>(U)b) & (U)1;
        t->ch[f] = add(t->ch[f], val, b-1);
        return t;
    }
    node* sub(node* t, U val, int b = B-1){
        assert(t);
        t->cnt--;
        if(t->cnt==0) return nullptr;
        if(b<0) return t;
        bool f = (val>>(U)b) & (U)1;
        t->ch[f] = sub(t->ch[f], val, b-1);
        return t;
    }
    U get_min(node* t, U val, int b = B-1) const{
        assert(t);
        if(b < 0) return 0;
        bool f = (val>>(U)b) & (U)1;
        f ^= !t->ch[f];
        return get_min(t->ch[f], val, b-1) | ((U)f<<(U)b);
    }

    node *root;
public:
    binary_trie() : root(nullptr){}
    int size() const{
        return root ? root->cnt : 0;
    }
    bool empty() const{
        return !root;
    }
    void insert(U val){
        root = add(root, val);
    }
    void erase(U val){
        root = sub(root, val);
    }
    U max_element(U bias = 0) const{
        return get_min(root, ~bias);
    }
    U min_element(U bias = 0) const{
        return get_min(root, bias);
    }
};
#endif