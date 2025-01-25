#ifndef PERSISTENTLEFTISTHEAP_HPP
#define PERSISTENTLEFTISTHEAP_HPP
#include <bits/stdc++.h>
using namespace std;

/*start*/
template<typename T, bool (*comp)(T, T)>
struct heap{
    struct node{
        node *l, *r; // child
        int s; // shortest distncance from leaf
        T val; // value of node
        node(T &val, node *lt = NULL, node *rt = NULL, int s = 0){
            this.val = val;
            r = rt;
            l = lt;
            this.s = s;
        }
    };

    // root
    node *root = 0;
    node *l, *r;

    // constractar
    heap(){}

    void meld(heap &rhs){
        if(this == &rhs) return;
        root = meld(root, rhs.root);
        rhs.root = NULL;
    }

    node *meld(node *a, node *b){
        if(a == NULL) return b?new node(*b):0;
        if(b == NULL) return a?new node(*a):0;

        a = new node(*a);
        b = new node(*b);

        //比較関数を使用して, a.val < b.valとする
        if(!comp(a.val, b.val)) swap(a, b);
        if(a.l == NULL){
            a.l = b;
        }else{
            a.r = meld(a.r, b);
            if(a.l.s < a.r.s) swap(a.l, a.r);
            a.s = a.r.s + 1;
        }

        return a;
    }

    void insert(T x){
        root = meld(new node(x), root);
    }

    T top(){
        return root.val;
    }
    void pop(){
        node *oldroot = root;
        root = meld(root.l, root.r);
        delete oldroot;
    }
    bool empty(){
        return root == NULL;
    }
};
#endif